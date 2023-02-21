#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <gtkmm.h>
#include <iostream>

class MyWindow : public Gtk::Window
{
	public:
		MyWindow();
	protected:
		bool openFile(GdkEventButton* event);

		Gtk::Box m_mainBox;
		Gtk::Box m_leftBox;
		Gtk::Box m_rightBox;
		Gtk::Separator m_separator;
		Gtk::EventBox m_eventBox;
		Gtk::Image m_image;
		Gtk::Label m_label;
		Gtk::Label m_label_test;
};

#endif
