#include "display.h"
#include "midi.h"
#include <MidiFile.h>

unsigned int nbWords(std::string text)
{
	int nbWords = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == ' ')
		{
			nbWords++;
		}
	}
	return nbWords/2;
}

unsigned int minOne(int num)
{
	return ((num <= 12) ? 12 : num);
}

MyWindow::MyWindow()
:
m_mainBox(Gtk::ORIENTATION_HORIZONTAL),
m_leftBox(Gtk::ORIENTATION_VERTICAL),
m_rightBox(Gtk::ORIENTATION_VERTICAL),
m_separator(Gtk::ORIENTATION_VERTICAL),
m_eventBox(),
m_image("./drop.png"),
m_label("Click or drop a MIDI file here"),
m_labelOutput("Output log"),
m_textView(),
m_labelChords("Chords in the midi file :"),
m_scrolledWindowLog(),
m_scrolledWindowChords(),
m_textViewChords(),
m_buttonClear()
{
	std::cout << get_style_context()->get_background_color(Gtk::STATE_FLAG_NORMAL).to_string() << std::endl;

	set_title("Decode MIDI");
	set_default_size(700, 400);
	set_resizable(false);
	add(m_mainBox);
	// mainBox
	m_mainBox.pack_start(m_leftBox);
	m_leftBox.set_name("leftBoxDrop");
	m_mainBox.pack_start(m_separator, false, false);
	m_separator.set_margin_top(15);
	m_separator.set_margin_bottom(15);
	m_mainBox.pack_start(m_rightBox);
	// leftBox
	m_eventBox.add(m_image);
	m_leftBox.pack_start(m_eventBox, false, false);
	m_eventBox.set_margin_top(60);
	m_eventBox.signal_button_press_event().connect(sigc::mem_fun(*this, &MyWindow::openFile));
	std::vector<Gtk::TargetEntry> listTargets = {Gtk::TargetEntry("text/uri-list")};
	m_eventBox.drag_dest_set(listTargets, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);
	m_eventBox.signal_drag_data_received().connect(sigc::mem_fun(*this, &MyWindow::dragFile));
	m_eventBox.signal_drag_motion().connect(sigc::mem_fun(*this, &MyWindow::removeHighlight));
	m_leftBox.pack_start(m_label, false, false);
	m_label.set_name("label");
	// rightBox
	m_rightBox.pack_start(m_labelOutput, false, false);
	m_rightBox.pack_start(m_scrolledWindowLog);
	m_scrolledWindowLog.add(m_textView);
	m_labelOutput.set_name("labelOutput");
	m_textView.set_name("textView");
	m_textView.set_editable(false);
	m_textView.set_cursor_visible(false);
	m_textView.set_wrap_mode(Gtk::WRAP_WORD);
	m_textView.set_margin_left(10);
	m_textView.set_margin_right(10);
	m_textView.set_margin_bottom(10);


	m_labelChords.set_name("labelChords");
	m_textViewChords.set_name("textViewChords");
	m_textViewChords.set_editable(false);
	m_textViewChords.set_cursor_visible(false);
	m_textViewChords.set_justification(Gtk::JUSTIFY_CENTER);
	m_textViewChords.set_wrap_mode(Gtk::WRAP_WORD);
	m_scrolledWindowLog.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	m_scrolledWindowChords.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	m_scrolledWindowChords.add(m_textViewChords);
	m_buttonClear.set_name("buttonClear");
	Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file("./trash.png");
	pixbuf = pixbuf->scale_simple(20, 20, Gdk::INTERP_BILINEAR);
	m_buttonClear.set_image(*Gtk::manage(new Gtk::Image(pixbuf)));
	m_buttonClear.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::changeLayout));

	show_all();
	present();
	m_image.get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
	// print the width of left box
	std::cout << "leftBox : " << m_leftBox.get_allocated_width() << std::endl;
	// print the width of right box
	std::cout << "rightBox : " << m_rightBox.get_allocated_width() << std::endl;

}

bool MyWindow::openFile(GdkEventButton* event)
{
	auto dialog = Gtk::FileChooserNative::create("Open MIDI file", *this, Gtk::FILE_CHOOSER_ACTION_OPEN, "Open", "Cancel");
	dialog->set_modal(true);
	dialog->set_transient_for(*this);
	Glib::RefPtr<Gtk::FileFilter> midiFilter = Gtk::FileFilter::create();
	midiFilter->set_name("MIDI Files");
	midiFilter->add_pattern("*.midi");
	midiFilter->add_pattern("*.mid");
	dialog->add_filter(midiFilter);
	int rtn = dialog->run();
	if (rtn == Gtk::RESPONSE_ACCEPT)
	{
		this->path = dialog->get_filename();
		changeLayout();
	}
	return true;
}

void MyWindow::dragFile(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time)
{
	std::string path = (char*)selection_data.get_data();
	path = path.substr(0, path.length() - 2);
	path = path.substr(7);
	int pos = path.find_last_of(".");
	std::string ext = path.substr(pos + 1);
	if (ext.compare("mid") == 0 || ext.compare("midi") == 0)
	{
		this->path = path;
		changeLayout();
	}
	else
	{
		Gtk::MessageDialog dialog(*this, "The file you drop don't get a .mid or .midi extension.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		dialog.set_name("dialogErr");
		Gtk::Label* label = (Gtk::Label*)dialog.get_message_area()->get_children()[0];
		label->set_name("labelErr");
		dialog.run();
	}
}

bool MyWindow::removeHighlight(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time)
{
	m_eventBox.drag_unhighlight();
	return false;
}

void MyWindow::changeLayout()
{
	if (drop)
	{
		Midi midi(path);
		smf::MidiFile midiFile;
		midiFile.read(path);
		midi.analyse(midiFile);

		windowSelection = new Gtk::Window();
		windowSelection->set_name("trackSelection");
		windowSelection->set_default_size(500, 300);
		windowSelection->set_modal(true);
		Gtk::HeaderBar* headerBar = new Gtk::HeaderBar();
		headerBar->set_title("Select tracks to analyse");
		Gtk::Button* validateButton = new Gtk::Button();
		validateButton->set_image_from_icon_name("gtk-apply", Gtk::ICON_SIZE_BUTTON);
		validateButton->signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::validateSelection));
		headerBar->pack_end(*validateButton);
		windowSelection->set_titlebar(*headerBar);
		Gtk::Box* box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
		windowSelection->add(*box);
		Gtk::ScrolledWindow* scrolledWindow = new Gtk::ScrolledWindow();
		scrolledWindow->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
		box->pack_start(*scrolledWindow, true, true);
		Gtk::ListBox* listBox = new Gtk::ListBox();
		listBox->set_selection_mode(Gtk::SELECTION_MULTIPLE);
		scrolledWindow->add(*listBox);	
		for (int i = 0; i < midiFile.getTrackCount(); i++)
		{
			Gtk::ListBoxRow* row = new Gtk::ListBoxRow();
			row->set_size_request(-1, 30);
			row->set_selectable(false);
			Gtk::Box* box = new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
			row->add(*box);
			Gtk::Label* label = new Gtk::Label(midi.getInstruments()[i]);
			box->pack_start(*label, true, true);
			Gtk::Switch* switchButton = new Gtk::Switch();
			switchButton->set_active(true);
			box->pack_start(*switchButton, false, false);
			listBox->add(*row);
		}
		windowSelection->show_all();
		windowSelection->present();
		while (windowSelection->is_visible())
			Gtk::Main::iteration();


		destroy(m_mainBox);
		destroy(m_leftBox);
		m_leftBox.set_name("leftBox");
		m_mainBox.pack_start(m_leftBox, false, false);
		m_mainBox.pack_start(m_separator, false, false);
		m_mainBox.pack_start(m_rightBox, true, true);
		m_leftBox.set_size_request(426, -1);
		Glib::RefPtr<Gtk::TextBuffer> buffer = m_textView.get_buffer();
		buffer->set_text("Test");
		m_leftBox.pack_start(m_labelChords, false, false);
		m_leftBox.pack_start(m_scrolledWindowChords);
		m_textViewChords.get_buffer()->set_text(midi.getChords());
		std::string text = m_textViewChords.get_buffer()->get_text();
		std::string font = "Arial ";
		int i = 35-(nbWords(text));
		std::cout << "i : " << i << std::endl;
		font += std::to_string(minOne(i));
		if (minOne(i) == 12)
			m_leftBox.set_name("leftBoxLarge");
		m_textViewChords.override_font(Pango::FontDescription(font));
		m_textView.get_buffer()->set_text(midi.getLog());
		m_leftBox.pack_start(m_buttonClear, false, false);
	} else
	{
		destroy(m_mainBox);
		destroy(m_leftBox);
		m_mainBox.pack_start(m_leftBox);
		m_mainBox.pack_start(m_separator, false, false);
		m_mainBox.pack_start(m_rightBox);
		m_leftBox.pack_start(m_eventBox, false, false);
		m_leftBox.pack_start(m_label, false, false);
		m_leftBox.set_name("leftBoxDrop");
	}
	drop = !drop;
	show_all();
	present();
}

void MyWindow::destroy(Gtk::Container& container)
{
	std::vector<Gtk::Widget*> children = container.get_children();
	for (auto child : children)
	{
		container.remove(*child);
	}
}

void MyWindow::validateSelection()
{
	windowSelection->hide();
}
