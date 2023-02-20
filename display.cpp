#include "display.h"

MyWindow::MyWindow()
:
m_mainBox(Gtk::ORIENTATION_HORIZONTAL),
m_leftBox(Gtk::ORIENTATION_VERTICAL),
m_rightBox(Gtk::ORIENTATION_VERTICAL),
m_separator(Gtk::ORIENTATION_VERTICAL),
m_image("drop.png"),
m_label("Drop a MIDI file here")
{
	set_title("Decode MIDI");
	set_default_size(700, 400);
	add(m_mainBox);
	m_mainBox.pack_start(m_leftBox);
	m_mainBox.pack_start(m_separator, false, false);
	m_separator.set_margin_top(15);
	m_separator.set_margin_bottom(15);
	m_mainBox.pack_start(m_rightBox);
	m_leftBox.pack_start(m_image);
	m_rightBox.pack_start(m_label);
	show_all();
	present();
}
