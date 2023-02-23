#include "display.h"

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
m_textView()
{
	set_title("Decode MIDI");
	set_default_size(700, 400);
	set_resizable(false);
	add(m_mainBox);
	// mainBox
	m_mainBox.pack_start(m_leftBox);
	m_leftBox.set_name("leftBox");
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
	m_rightBox.pack_start(m_textView);
	m_labelOutput.set_name("labelOutput");
	m_textView.set_name("textView");
	m_textView.set_editable(false);
	m_textView.set_cursor_visible(false);
	m_textView.set_margin_left(10);
	m_textView.set_margin_right(10);
	m_textView.set_margin_bottom(10);

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
	return true;
}

void MyWindow::dragFile(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time)
{
	std::cout << "dragFile" << std::endl;
}

bool MyWindow::removeHighlight(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time)
{
	m_eventBox.drag_unhighlight();
	return false;
}
