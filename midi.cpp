#include "midi.h"

Midi::Midi(std::string path)
{
	this->path = path;
	this->log = "";
}

std::string Midi::removeLastZeros(std::string str)
{
	while (str.back() == '0' || str.back() == '.')
		str.pop_back();
	return str;
}

bool Midi::contains(std::vector<std::string> vec, std::string str)
{
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == str)
			return true;
	return false;
}

bool Midi::contains(std::vector<int> vec, int num)
{
	for (int i = 0; i < vec.size(); i++)
		if (vec[i]%12 == num)
			return true;
	return false;
}

int Midi::findSmallest(std::vector<int> vec)
{
	int smallest = vec[0];
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] < smallest)
			smallest = vec[i];
	return smallest;
}

std::string Midi::numToName(int num)
{
	std::string notes[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	return notes[num % 12];
}

std::string Midi::tickToPulse(int tick)
{
	std::string str = std::to_string(tick / 1920 + 1);
	str.append(" measure");
	if (tick % 1920 != 0)
	{
		tick %= 1920;
		str.append(" and the ");
		str.append(removeLastZeros(std::to_string(((float) tick / 480)+1)));
		str.append(" beat");
	}
	return str;
}

void Midi::computeChord()
{
	int root = findSmallest(notes);
	std::string chord(numToName(root));
	// find 3rd
	bool minor = false;
	if (contains(notes, (root+3)%12))
		minor = true;
	// 1st inversion
	if (minor && contains(notes, (root+8)%12)) {
		root = (root+8)%12;
		chord.append("/");
		chord.append(numToName(root));
	} else if (!minor && !contains(notes, (root+5)%12) && contains(notes, (root+9)%12)) {
		root = (root+9)%12;
		chord.append("/");
		chord.append(numToName(root));
	}
	// 2nd inversion
	if (contains(notes, (root+5)%12)) {
		if (contains(notes, (root+8)%12)) {
			root = (root+5)%12;
			chord.append("/");
			chord.append(numToName(root));
		} else if (contains(notes, (root+9)%12)) {
			root = (root+5)%12;
			chord.append("/");
			chord.append(numToName(root));
		}
	}
	// extra notes
	for (int i = 0; i < notes.size(); i++)
		chord.append(extaNotes[(notes[i] - root) % 12]);
	if (!contains(chords, chord))
		chords.push_back(chord);
}

void Midi::analyse(smf::MidiFile& midifile)
{
	for (int j = 0; j < midifile.getTrackCount(); j++)
	{
		log.append("--- Track n°").append(std::to_string(j)).append("---").append("\n");
		for (int i = 0; i < midifile[j].size(); i++)
		{
			int num = midifile[j][i].getKeyNumber();
			// si les notes s'enlevent en même temps, il ne faut pas compute
			if (lastTick != midifile[j][i].tick && currentNoteOn > 2)
				computeChord();
			if (midifile[j][i].isTempo())
				log.append("BPM : ").append(std::to_string(midifile[j][i].getTempoBPM())).append("\n");
			if (midifile[j][i].isNoteOn()) {
				log.append("Note On : ").append(numToName(num).append(std::to_string(num / 12))).append(" at the ").append(tickToPulse(midifile[j][i].tick)).append(".\n");
				currentNoteOn++;
				notes.push_back(midifile[j][i].getKeyNumber());
			}
			if (midifile[j][i].isNoteOff()) {
				log.append("Note Off : ").append(numToName(num).append(std::to_string(num / 12))).append(" at the ").append(tickToPulse(midifile[j][i].tick)).append(".\n");
				currentNoteOn--;
				notes.erase(std::remove(notes.begin(), notes.end(), num), notes.end());
			}
			lastTick = midifile[j][i].tick;
		}
		notes.clear();
	}
}

std::string Midi::getChords()
{
	std::string str;
	for (int i = 0; i < chords.size(); i++) {
		str.append(chords[i]);
		if (i != chords.size() - 1)
			str.append(" - ");
	}
	return str;
}

std::string Midi::getLog()
{
	return log;
}
