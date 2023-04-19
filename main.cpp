#include <iostream>
#include <MidiFile.h>
#include "display.h"
#include "midi.h"

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		auto app = Gtk::Application::create("org.gtkmm.examples.base");
		MyWindow window;
		Glib::RefPtr<Gtk::CssProvider> css = Gtk::CssProvider::create();
		css->load_from_path("style.css");
		window.get_style_context()->add_provider_for_screen(Gdk::Screen::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_USER);
		return app->run(window);
	}
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <midifile>" << std::endl;
		return EXIT_FAILURE;
	}
	smf::MidiFile midifile;
	midifile.read(argv[1]);
	if (!midifile.status())
	{
		std::cerr << "Error reading MIDI file " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}
	Midi midi(argv[1]);
	midi.analyse(midifile);
	std::cout << midi.getLog() << std::endl;
	std::string chords = midi.getChords();
	std::cout << "All chords passed : " << chords <<  std::endl;
	return EXIT_SUCCESS;
}
