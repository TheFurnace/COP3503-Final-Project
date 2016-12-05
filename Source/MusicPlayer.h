#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<Windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include "Index.h"
using namespace std;
#pragma comment(lib, "Winmm.lib")
#define SONG_TOKEN "song"


class MusicPlayer {
	string filePath;
	bool isOpen;

	TrackList tracklist;

	int integer;
public:
	MusicPlayer();
	void Open(string newFilePath);
	void Open(Track newTrack);
	void MusicPlayer::PlayPause(Track newTrack);
	void Play();
	void Pause();
	void Resume();
	void Stop();

	void Open(TrackList in, int index);
	void Previous();
	void Next();


	std::wstring s2ws(const std::string& s);
};


