#include <iostream>
#include <MidiFile.h>

using namespace smf;

unsigned int currentNoteOn = 0;
unsigned int lastTick = 0;
std::vector<int> notes;
std::vector<std::string> chords;
std::string extaNotes[] = { "", "b9", "9", "m", "", "sus4", "#11", "", "#5", "6", "7", "△7" };

bool contains(std::vector<std::string> vec, std::string str)
{
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == str)
			return true;
	return false;
}

int findSmallest(std::vector<int> vec)
{
	int smallest = vec[0];
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] < smallest)
			smallest = vec[i];
	return smallest;
}

std::string numToName(int num)
{
	std::string notes[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	return notes[num % 12].append(std::to_string(num / 12));
}

std::string tickToPulse(int tick)
{
	std::string str = std::to_string(tick / 1920 + 1);
	str.append(" measure");
	if (tick % 1920 != 0)
	{
		tick %= 1920;
		str.append(" and the ");
		str.append(std::to_string(tick / 480));
		str.append(" beat");
	}
	return str;
}

void computeChord()
{
	int root = findSmallest(notes);
	std::string chord(1,numToName(root).front());
	for (int i = 0; i < notes.size(); i++)
		chord.append(extaNotes[(notes[i] - root) % 12]);
	if (!contains(chords, chord))
		chords.push_back(chord);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <midifile>" << std::endl;
		return EXIT_FAILURE;
	}
	MidiFile midifile;
	midifile.read(argv[1]);
	if (!midifile.status())
	{
		std::cerr << "Error reading MIDI file " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}
	for (int i = 0; i < midifile[0].size(); i++)
	{
		// si les notes s'enlevent en même temps, il ne faut pas compute
		if (lastTick != midifile[0][i].tick && currentNoteOn > 2)
			computeChord();
		if (midifile[0][i].isTempo())
			std::cout << "BPM : " << midifile[0][i].getTempoBPM() << std::endl;
		if (midifile[0][i].isNoteOn()) {
			std::cout << "Note On : " << numToName(midifile[0][i].getKeyNumber()) << " at the " << tickToPulse(midifile[0][i].tick) << std::endl;
			currentNoteOn++;
			notes.push_back(midifile[0][i].getKeyNumber());
		}
		if (midifile[0][i].isNoteOff()) {
			std::cout << "Note Off : " << numToName(midifile[0][i].getKeyNumber()) << " at the " << tickToPulse(midifile[0][i].tick) << std::endl;
			currentNoteOn--;
			notes.erase(std::remove(notes.begin(), notes.end(), midifile[0][i].getKeyNumber()), notes.end());
		}
		lastTick = midifile[0][i].tick;
	}
	std::cout << "All chords passed : " << std::endl;
	for (int i = 0; i < chords.size(); i++) {
		std::cout << chords[i];
		if (i != chords.size() - 1)
			std::cout << ", ";
		else
			std::cout << std::endl;
	}
	return EXIT_SUCCESS;
}
