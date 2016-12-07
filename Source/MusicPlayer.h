#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include "Index.h"

#include "MusicPlayer.h"
#include <tag.h>
#include <mpegfile.h>
#include <attachedpictureframe.h>
#include <id3v2tag.h>
#include <mp4file.h>
#include <mp4tag.h>
#include <mp4coverart.h>
#include <fileref.h>
using namespace std;
#pragma comment(lib, "Winmm.lib")
#define SONG_TOKEN "song"


class MusicPlayer {
	string filePath_;
	bool isOpen_;
	bool isPlaying_;
	bool isPaused_;

	MetadataWorker meta_;
	TrackList* tracklist_;

	int currentSong_;
public:
	MusicPlayer();
	void Open(string newFilePath);
	void Open(Track* newTrack);
	void MusicPlayer::PauseResume();
	void Play();
	void Pause();
	void Resume();
	void Stop();

	void Open(TrackList* in, int index);
	void Play(int songIndex);
	void Previous();
	void Next();

	std::wstring s2ws(const std::string& s);
};




