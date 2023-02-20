#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <gtkmm.h>

class MyWindow : public Gtk::Window
{
	public:
		MyWindow();
	protected:
		Gtk::Box m_mainBox;
		Gtk::Box m_leftBox;
		Gtk::Box m_rightBox;
		Gtk::Label m_Label;
};

#endif
