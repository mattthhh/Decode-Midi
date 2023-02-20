#include "display.h"

MyWindow::MyWindow()
:
m_mainBox(Gtk::ORIENTATION_HORIZONTAL),
m_leftBox(Gtk::ORIENTATION_VERTICAL),
m_rightBox(Gtk::ORIENTATION_VERTICAL),
m_Label("Hello, World!")
{
	set_title("Decode MIDI");
	set_default_size(700, 400);
	add(m_mainBox);
	m_mainBox.pack_start(m_leftBox);
	m_mainBox.pack_start(m_rightBox);
	m_leftBox.pack_start(m_Label);
	show_all();
	present();
}
