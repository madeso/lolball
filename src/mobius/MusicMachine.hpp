#ifndef MUSIC_MACHINE_HPP
#define MUSIC_MACHINE_HPP

#include "fmod.hpp"
#include <string>

#include <vector>

struct Song {
	Song(const std::string& pFile, const unsigned int pLength);
	std::string file;
	unsigned int length;
};

class MusicMachine {
public:
	MusicMachine();
	~MusicMachine();

	void loadAndPlaySongs();
	void step();
	void setVolume(float pNewVolume);

	static MusicMachine& getInstance();
	static MusicMachine* getInstancePtr();
protected:
	void loadSongs();
	void playSongs();

	void initError(FMOD_RESULT pCause) const;

	void playNextSong();

	void stopSong();
private:
	std::vector<Song> mSongs;
	FMOD::Sound* mMusic;
	FMOD::Channel* mChannel;
	std::size_t mIndex;
	unsigned int mSongLength;
	float mVolume;

	static MusicMachine* sInstance;
};

#endif