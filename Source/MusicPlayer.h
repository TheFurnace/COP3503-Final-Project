#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<Windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
using namespace std;
#pragma comment(lib, "Winmm.lib")


class MusicPlayer{
	private string filePath;
	private bool isOpen;
	private bool isPlaying;
	
	public:
		MusicPlayer();
		void Play(string filePath);
		void Pause(string filePath);
		void Resume(string filePath);
		void Stop(string filePath);
		std::wstring s2ws(const std::string& s);
};


