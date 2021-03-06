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
	:isOpen_(false), isPlaying_(false), isPaused_(false)
{
}
void MusicPlayer::Open(string newFilePath)
{
	filePath_ = newFilePath;

	ostringstream os;
	//L"open \" + file path and name + \"....
	os << "open \"" << filePath_ << "\" type MPEGvideo alias song";
	wstring fullInput = s2ws(os.str());
	LPCWSTR a = fullInput.c_str();

	if (isOpen_)
		Stop();

	mciSendStringW(a, NULL, 0, NULL);
	isOpen_ = true;

	Play();
}
void MusicPlayer::Open(Track* newTrack)
{
	filePath_ = newTrack->getPath();

	if (isOpen_)
		Stop();


	ostringstream os;
	//L"open \" + file path and name + \"....
	os << "open \"" << filePath_ << "\" type MPEGvideo alias song";
	wstring fullInput = s2ws(os.str());
	LPCWSTR a = fullInput.c_str();

	mciSendStringW(a, NULL, 0, NULL);
	isOpen_ = true;

	Play();
	currentTrack_ = newTrack;
}
//For use as a single button
void MusicPlayer::PauseResume()
{
	if (!isPaused_) {
		Pause();
	}
	else if (isPaused_) {
		Resume();
	}
}
//Opens and plays a new song given a filepath input
void MusicPlayer::Play()
{
	if (isOpen_)
	{
		//Play the song
		mciSendString("play song", NULL, 0, NULL);
		isPlaying_ = true;
	}
}
//Pauses a song at filepath input
void MusicPlayer::Pause()
{
	mciSendString("pause song", NULL, 0, NULL);
	isPaused_ = true;
}
//Resumes a song at filepath input
void MusicPlayer::Resume()
{
	mciSendString("resume song", NULL, 0, NULL);
	isPaused_ = false;
}
//Closes a song at filepath input
void MusicPlayer::Stop()
{
	if (isOpen_) {
		mciSendString("close song", NULL, 0, NULL);
		isOpen_ = false;
		isPlaying_ = false;
		isPaused_ = false;
	}
}

void MusicPlayer::Open(TrackList* in, int index)
{
	tracklist_ = in;
	Play(0);
}

void MusicPlayer::Play(int songIndex)
{
	currentSong_ = songIndex;

	/*if (!(currentSong_ < tracklist_->Size()))
	{
		Open(tracklist_->GetTrack(currentSong_));

		Sleep(meta_.GetTrackLength() * 1000);

		Next();
	}*/
	Open(tracklist_->GetTrack(currentSong_));
}

void MusicPlayer::Previous()
{
	if (isPlaylist_)
		Play(currentSong_ - 1);
	else
		cout << "Not a playlist" << endl;
}

void MusicPlayer::Next()
{
	if(isPlaylist_)
		Play(currentSong_ + 1);
	else
		cout << "Not a playlist" << endl;
}


string MetadataWorker::GetTitle() {

	TagLib::FileRef f(filedir.c_str());

	if (!f.isNull() && f.tag()) {

		TagLib::Tag *tag = f.tag();

		if (tag->title() != "") {

			return tag->title().toCString();

		}
		else {
			return "No Title";

		}
	}
	return "No File";
}

void MetadataWorker::SetFileDir(string dirarg) {

	filedir = dirarg;
}

string MetadataWorker::GetAlbum()
{

	TagLib::FileRef f(filedir.c_str());

	if (!f.isNull() && f.tag()) {

		TagLib::Tag *tag = f.tag();

		if (tag->album() != "") {

			return tag->album().toCString();

		}
		else {
			return "No Album";

		}
	}
	return "No File";
}

string MetadataWorker::GetArtist() {

	TagLib::FileRef f(filedir.c_str());

	if (!f.isNull() && f.tag()) {

		TagLib::Tag *tag = f.tag();

		if (tag->artist() != "") {

			return tag->artist().toCString();

		}
		else {
			return "No Artist";

		}
	}
	return "No File";
}

string MetadataWorker::GetYear() {

	TagLib::FileRef f(filedir.c_str());

	if (!f.isNull() && f.tag()) {

		TagLib::Tag *tag = f.tag();

		if (tag->year() != NULL) {

			return to_string(tag->year());

		}
		else {
			return "No Year";

		}
	}
	return "No File";
}

string MetadataWorker::GetTrackNum() {

	TagLib::FileRef f(filedir.c_str());

	if (!f.isNull() && f.tag()) {

		TagLib::Tag *tag = f.tag();

		if (tag->track() != 0) {

			return to_string(tag->track());

		}
		else {
			return 0;

		}
	}
	return "No File";
}

int MetadataWorker::GetTrackLength()
{
	TagLib::FileRef f(filedir.c_str());

	if (!f.isNull() && f.tag()) {

		return f.audioProperties()->lengthInSeconds();
	}
	return 0;
}



//To change songs, pause current, close current, open next/previous, play next/previous
