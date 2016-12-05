#pragma once
#include "TrackList.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#define CONFIG_FILE "config.txt"
using namespace std;

class Index
{
	const string ext;
	string trackIndexLocation_;
	string tracklistIndexLocation_;
	vector<string> directories_;
	vector<int> unusedID_;
	int lastID_;

public:
	Index();
	~Index();

	void InitiateIndexFromConfig();
	void WriteFieldsToConfig();

	void AddDirectory(string newDirectory) { directories_.push_back(newDirectory); }
	string GetDirectory(int index) { return directories_.at(index); }
	int GetDirectorySize() { return directories_.size(); }
	void DeleteDirectory(string rmDirectory);

	TrackList* ReadMainIndex();
	vector<TrackList> ReadTrackListIndex();

	void WriteTrackListIndex(vector<TrackList> in);
	void WriteTrackIndex();
	void UpdateTrackIndex();

	vector<string> GetAllEntries();

	bool isInVector(string input, vector<string> vector);

	TrackList TrackListFromIDList(vector<int> idList, string name);


	int q = 0;
};

class MetadataWorker
{
	string filedir;

public:
	MetadataWorker(string filedir);

	void setFileDir();

	string GetTitle();
	string GetAlbum() 
	string GetArtist();
	string GetYear();
}