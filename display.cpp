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
m_label_test("louuurd")
{
	set_title("Decode MIDI");
	set_default_size(700, 400);
	set_resizable(false);
	add(m_mainBox);
	m_mainBox.pack_start(m_leftBox);
	m_leftBox.set_name("leftBox");
	m_mainBox.pack_start(m_separator, false, false);
	m_separator.set_margin_top(15);
	m_separator.set_margin_bottom(15);
	m_mainBox.pack_start(m_rightBox);
	m_eventBox.add(m_image);
	m_leftBox.pack_start(m_eventBox, false, false);
	m_eventBox.set_margin_top(60);
	m_eventBox.signal_button_press_event().connect(sigc::mem_fun(*this, &MyWindow::openFile));
	m_leftBox.pack_start(m_label, false, false);
	m_label.set_name("label");
	m_rightBox.pack_start(m_label_test);
	show_all();
	present();
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
