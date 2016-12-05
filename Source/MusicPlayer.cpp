#include "MusicPlayer.h"


std::wstring s2ws(const std::string& s)
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
int main(int argc, char* argv[]){
	//Change this string to whatever file you want to test
	string locationOfMusic = "C:\\Users\\nc1\\Downloads\\Boombox_Cartel_-_Dia_De_Los_Muertos_Mix.mp3";
	//Placeholder menu for gui buttons
	while(playerIsOpen){
		cout << "Media Controls" << endl;
		cout << "1. Play" << endl;
		cout << "2. Pause" << endl;
		cout << "3. Resume" << endl;
		cout << "4. Stop" << endl << endl;
		int choice;
		cin >> choice;
		switch(choice){
			case 1://Play
				//if(!isPlaying){
					Play(locationOfMusic);
				//}
				break;
			case 2://Pause
				if(isPlaying){
					Pause(locationOfMusic);
				}
				break;
			case 3://Resume
				if(!isPlaying) Resume(locationOfMusic);
				break;
			case 4://Stop
				Stop(locationOfMusic);
				break;
			default:
				playerIsOpen = false;
				break;
		}

	}

}
MusicPlayer::MusicPlayer(){
	playerIsOpen = true;
	isOpen = false;
	isPlaying = false;
}
//Opens and plays a new song given a filepath input
void MusicPlayer::Play(string filePath){
	ostringstream os;
	//L"open \" + file path and name + \"....
	os << "open \"" << filePath << "\" type MPEGvideo alias song";
	LPCWSTR a = s2ws(os.str().c_str());
	mciSendStringW(a, NULL, 0, NULL);
	//Play the song
	mciSendString("Play " + filePath, NULL, 0, NULL);
	isOpen = true;
}
//Pauses a song at filepath input
void MusicPlayer::Pause(string filePath){
	mciSendString("Pause " + filePath, NULL, 0, NULL);
	isPlaying = false;
}
//Resumes a song at filepath input
void MusicPlayer::Resume(string filePath){
	mciSendString("Resume " + filePath, NULL, 0, NULL);
	isPlaying = true;
}
//Closes a song at filepath input
void MusicPlayer::Stop(string filePath){
	if(isOpen){
		mciSendString("Close " + filePath, NULL, 0, NULL);
		isOpen = false;
	}
}
//To change songs, pause current, close current, open next/previous, play next/previous
