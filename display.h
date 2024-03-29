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
		void dragFile(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
		bool removeHighlight(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time);
		void changeLayout();
		void destroy(Gtk::Container& container);
		void validateSelection();

		std::vector<int> selectedTracks;
		std::string path;
		bool drop = true;

		Gtk::Box m_mainBox;
		Gtk::Box m_leftBox;
		Gtk::Box m_rightBox;
		Gtk::Separator m_separator;
		Gtk::EventBox m_eventBox;
		Gtk::Image m_image;
		Gtk::Label m_label;
		Gtk::Label m_labelOutput;
		Gtk::TextView m_textView;
		Gtk::Label m_labelChords;
		Gtk::ScrolledWindow m_scrolledWindowLog;
		Gtk::ScrolledWindow m_scrolledWindowChords;
		Gtk::TextView m_textViewChords;
		Gtk::Button m_buttonClear;
		Gtk::Window *windowSelection;
		Gtk::ListBox *listBox;
};

#endif
