#include "MusicMachine.hpp"

#include "Exceptions.hpp"
#include "FmodSystem.hpp"
#include <sstream>
#include "fmod_errors.h"
#include <cassert>

#include "File.hpp"
#include "tinyxml.h"

#include "DataPointer.hpp"
#include "math.hpp"

Song::Song(const std::string& pFile, const unsigned int pLength) : file(pFile), length(pLength) {
}

void MusicMachine::initError(FMOD_RESULT pCause) const {
	if (pCause != FMOD_OK) 	{
		std::ostringstream str;
		str << "Fmod error! (" << pCause << ") " << FMOD_ErrorString(pCause);
		throw InitException(str.str());
	}
}

MusicMachine::MusicMachine() : mMusic(0), mChannel(0), mIndex(0), mSongLength(0), mVolume(1.0f) {
	assert( !sInstance );
	sInstance = this;
}

MusicMachine::~MusicMachine() {
	stopSong();
	assert( sInstance );
	sInstance = 0;
}

void MusicMachine::stopSong() {
	mChannel = 0;
	if( mMusic ) {
		mMusic->release();
		mMusic = 0;
	}
}

void MusicMachine::loadAndPlaySongs() {
	loadSongs();
	playSongs();
}
void MusicMachine::loadSongs() {
	std::istringstream file;
	loadFileIntoString("songs.xml", &file);
	TiXmlDocument doc; doc.Parse(file.str().c_str());
	{
		TiXmlHandle docHandle( &doc );
		for(TiXmlElement* entity = docHandle.FirstChild("music").FirstChild("song").ToElement(); entity; entity = entity->NextSiblingElement("song") ) {
			const char* filePointer = entity->Attribute("file");
			const std::string file = filePointer?filePointer:"";
			const char* lengthStr = entity->Attribute("length");
			unsigned int length = 0;
			if( lengthStr ) {
				std::istringstream str(lengthStr);
				str >> length;
			}
			mSongs.push_back( Song(file, length) );
		}
	}
	//mSongs.push_back("KY_404.XM");
}
void MusicMachine::playSongs() {
	playNextSong();
}
void MusicMachine::playNextSong() {
	stopSong();
	if( mSongs.size() > 0 ) {
		const std::string file = "../music/" + mSongs[mIndex].file;
		initError(FmodSystem::getInstance().getSystem()->createStream(file.c_str(), FMOD_DEFAULT, 0, &mMusic));

		initError( FmodSystem::getInstance().getSystem()->playSound(FMOD_CHANNEL_FREE, mMusic, false, &mChannel) );

		mSongLength = mSongs[mIndex].length;

		mIndex++;
		if( mIndex >= mSongs.size() ) {
			mIndex = 0;
		}
	}
}
void MusicMachine::step() {
	bool isPlaying = false;
	if( mChannel ) {
		initError(mChannel->isPlaying(&isPlaying));

		unsigned int position = 0;
		initError( mChannel->getPosition(&position, FMOD_TIMEUNIT_MS) );

		bool stepNext = false;
		if( mSongLength > 0 ) {
			if (position > mSongLength) {
				stepNext = true;
			}
		}
		if( stepNext || !isPlaying  ) {
			playNextSong();
		}
	}

	initError(mChannel->setVolume(mVolume));

	// change master
	FMOD::ChannelGroup* group = 0;
	initError(FmodSystem::getInstance().getSystem()->getMasterChannelGroup(&group));
	DataPointer<real> master("master");
	initError( group->setVolume( master.get() ) );
}

void MusicMachine::setVolume(float pNewVolume) {
	mVolume = pNewVolume;
}

MusicMachine& MusicMachine::getInstance() {
	return *getInstancePtr();
}
MusicMachine* MusicMachine::getInstancePtr() {
	assert(sInstance);
	return sInstance;
}

MusicMachine* MusicMachine::sInstance = 0;