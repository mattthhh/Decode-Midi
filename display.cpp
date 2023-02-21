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
	add(m_mainBox);
	m_mainBox.pack_start(m_leftBox);
	m_mainBox.pack_start(m_separator, false, false);
	m_separator.set_margin_top(15);
	m_separator.set_margin_bottom(15);
	m_mainBox.pack_start(m_rightBox);
	m_eventBox.add(m_image);
	m_leftBox.pack_start(m_eventBox, false, false);
	m_leftBox.set_margin_top(80);
	m_eventBox.signal_button_press_event().connect(sigc::mem_fun(*this, &MyWindow::openFile));
	m_leftBox.pack_start(m_label, false, false);
	m_label.set_margin_bottom(15);
	m_rightBox.pack_start(m_label_test);
	show_all();
	present();
}

bool MyWindow::openFile(GdkEventButton* event)
{
	std::cout << "Open file" << std::endl;
	return true;
}
