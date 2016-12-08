#include "SMP.h"
#include <sstream>
#include <vector>
#include "boost\algorithm\string.hpp"
#include "boost\filesystem.hpp"

using boost::iequals;

//global vars
Index global;
MusicPlayer mci;
Track* nowPlayingTrack;
TrackList* nowPlayingList;
bool isNowPlayingTrack;
bool isNowPlayingList;
bool thereAreDirectories;


int main(int argc, char * argv[])
{
	if (cmdOptionExists(argv, argv + argc, "-h"))
	{
		cout << "help text" << "\n";
	}

	char * filename = getCmdOption(argv, argv + argc, "-f");

	if (filename)
	{
		cout << "add " << filename << " to tracklist\n";
	}

	if (global.GetDirectorySize() == 0)
		thereAreDirectories = false;
	else
		thereAreDirectories = true;

	startTBI();

	return 0;
}


char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

int startTBI()
{
	cout << "Welcome to Music Player\n";

	/*
	if !(tracklist) {
	cout << "No music directory input, please input a music file directory: ";
	cin >> input
	check if file is viable and populate tracklist
	*/

	while (true)
	{
		cout << ">>";

		char input[256];
		string token[2];

		cin.getline(input,256);
		istringstream is(input);

		is >> token[0];

		if (iequals(token[0], "play")) { // Use boost::iequals
			is >> token[0];
			is >> token[1];
			if (isInteger(token[0]))
			{
				if (global.main_->isTrackID(stoi(token[0])))
				{
					cout << "Play track " << token[0] << endl;
					mci.Open(global.main_->search(stoi(token[0])));
					nowPlayingTrack = global.main_->search(stoi(token[0]));
					isNowPlayingList = false;
					isNowPlayingTrack = true;
				}
			}
			else if (iequals(token[0], "main") && token[1].length() == 0)
			{
				mci.Open(global.main_, 0); 
				isNowPlayingList = true;
			}
			else if (iequals(token[0], "main") && isInteger(token[1]))
			{
				if (stoi(token[1]) <= global.main_->Size() && stoi(token[1]) > 0)
				mci.Open(global.main_, stoi(token[1]));
				isNowPlayingList = true;
			}
			else if (global.isPlaylist(token[0]) && token[1].length() == 0)
			{
				mci.Open(global.GetPlaylist(token[0]), 0);
				isNowPlayingList = true;
			}
			else if (global.isPlaylist(token[0]) && isInteger(token[1]))
			{
				if (stoi(token[1]) <= global.GetPlaylist(token[0])->Size() && stoi(token[1]) > 0)
				{
					mci.Open(global.GetPlaylist(token[0]), stoi(token[1]));
					isNowPlayingList = true;
				}
			}
			else
				BadInput();
		}
		else if (iequals(token[0], "pause")) {
			cout << "Pause track\n";
			mci.Pause();
		}
		else if (iequals(token[0], "resume")) {
			cout << "Pause track\n";
			mci.Resume();
		}
		else if (iequals(token[0], "next")) {
			cout << "Next track\n";
			mci.Next();
		}
		else if (iequals(token[0], "previous")) {
			cout << "Previous track\n";
			mci.Previous();
		}
		else if (iequals(token[0], "mkplaylist")) {
			is >> token[0];
			if (!global.isPlaylist(token[0]))
				global.AddPlaylist(token[0]);
			else
				BadInput();
		}
		else if (iequals(token[0], "rmplaylist")) {
			is >> token[0];
			if (global.RmPlaylist(token[0]))
				cout << "Remove playlist " << token[0] << "\n";
			else
				BadInput();
		}
		else if (iequals(token[0], "directory"))
		{
			is >> token[0];
			if (iequals(token[0], "add"))
			{
				is >> token[0];
				if (boost::filesystem::exists(token[0]))
					global.AddDirectory(token[0]);
				else
					BadInput();
			}
			else if (iequals(token[0], "remove"))
			{
				is >> token[0];
				if (boost::filesystem::exists(token[0]))
					global.DeleteDirectory(token[0]);
				else
					BadInput();
			}
			else
				BadInput();
		}
		else if (iequals(token[0], "displaylist")) {
			is >> token[0];
			if (global.isPlaylist(token[0]))
				cout << global.GetPlaylist(token[0])->String();
			else if (iequals("main", token[0]))
				cout << global.main_->String();
			else
				BadInput();
		}
		else if (iequals(token[0],"manage")) {
			is >> token[0];
			if (iequals(token[0], "track"))
			{
				is >> token[0];
				is >> token[1];
				if (isInteger(token[0]) && global.isPlaylist(token[1]))
				{
					if (global.main_->isTrackID(stoi(token[0])))
					{
						global.GetPlaylist(token[1])->AddTrack(global.main_->search(stoi(token[0])));
					}
					else
						BadInput();
				}
				else
					BadInput();
			}
			else if (iequals(token[0], "playlist"))
			{
				is >> token[0];
				if (global.isPlaylist(token[0]))
				{
					is >> token[1];
					if (iequals(token[1], "remove"))
					{
						global.RmPlaylist(token[0]);
					}
					else if (iequals(token[1], "rename"))
					{
						is >> token[1];
						global.GetPlaylist(token[0])->SetName(token[1]);
					}
					else
						BadInput();
				}
				else
					BadInput();
			}
			else
				BadInput();
		}
		else if (iequals(token[0], "search")) {
			is >> token[0];
			is >> token[1];

			if (token[0].size() != 0 && token[1].size() != 0)
			{
				global.RmPlaylist("search result");
				global.AddPlaylist(global.main_->search(token[0], token[1]));
			}
			else
				BadInput();
		}
		else if (iequals(token[0], "nowplaying")) {
			is >> token[0];
			if (token[0].length() == 0)
				cout << nowPlayingTrack->String();
			else if (iequals(token[0], "list"))
				cout << nowPlayingList->String();
			else
				BadInput();
		}
		else if (iequals(token[0], "exit") || iequals(token[0], "q")) {
			break;
		}
		else {
			BadInput();
		}
	}
	return 0;
}

//Borrowed code
//http://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool isInteger(const std::string & s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

void BadInput()
{
	cout << "Unknown option, type \"help\" for help\n";
}