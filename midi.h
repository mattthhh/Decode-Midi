#ifndef MIDI_H_
#define MIDI_H_

#include <iostream>
#include <MidiFile.h>

class Midi
{
	public:
		Midi(std::string path);
		void analyse(smf::MidiFile& midifile);
		std::string getChords();
		std::string getLog();
	protected:
		std::string path;
		std::string log;

		unsigned int currentNoteOn = 0;
		unsigned int lastTick = 0;
		std::vector<int> notes;
		std::vector<std::string> chords;
		std::string extaNotes[12] = { "", "b9", "9", "m", "", "sus4", "#11", "", "#5", "6", "7", "△7" };

		std::string removeLastZeros(std::string str);
		bool contains(std::vector<std::string> vec, std::string str);
		bool contains(std::vector<int> vec, int num);
		int findSmallest(std::vector<int> vec);
		std::string numToName(int num);
		std::string tickToPulse(int tick);
		void computeChord();
};

#endif