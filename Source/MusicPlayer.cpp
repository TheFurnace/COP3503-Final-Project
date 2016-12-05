#include "MusicPlayer.h"


std::wstring MusicPlayer::s2ws(const std::string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
MusicPlayer::MusicPlayer() 
{
	isOpen = false;
}
void MusicPlayer::Open(string newFilePath)
{
	filePath = newFilePath;

	ostringstream os;
	//L"open \" + file path and name + \"....
	os << "open \"" << filePath << "\" type MPEGvideo alias song";
	wstring fullInput = s2ws(os.str());
	LPCWSTR a = fullInput.c_str();

	if (isOpen)
		Stop();

	mciSendStringW(a, NULL, 0, NULL);
	isOpen = true;
}
void MusicPlayer::Open(Track newTrack)
{
	filePath = newTrack.getPath();

	ostringstream os;
	//L"open \" + file path and name + \"....
	os << "open \"" << filePath << "\" type MPEGvideo alias song";
	wstring fullInput = s2ws(os.str());
	LPCWSTR a = fullInput.c_str();

	if (isOpen)
		Stop();

	mciSendStringW(a, NULL, 0, NULL);
	isOpen = true;
}
//For use as a single button
void MusicPlayer::PlayPause(Track newTrack)
{
	if(!isOpen){
		Open(newTrack);
	}else if(isPlaying){
		Pause();
	}else if(!isPlaying){
		Resume();
	}
}
//Opens and plays a new song given a filepath input
void MusicPlayer::Play() 
{
	if (isOpen)
	{
		//Play the song
		mciSendString("play song" , NULL, 0, NULL);
	}
}
//Pauses a song at filepath input
void MusicPlayer::Pause() 
{
		mciSendString("pause song" , NULL, 0, NULL);
}
//Resumes a song at filepath input
void MusicPlayer::Resume() 
{
		mciSendString("resume song" , NULL, 0, NULL);
}
//Closes a song at filepath input
void MusicPlayer::Stop() 
{
	if (isOpen) {
		mciSendString("close song" , NULL, 0, NULL);
		isOpen = false;
	}
}

//To change songs, pause current, close current, open next/previous, play next/previous
