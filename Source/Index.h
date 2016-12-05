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
	//The extension for the file type being used (mp3)
	const string ext;
	//location of track index (stores metadata for all songs)
	string trackIndexLocation_;
	//location of playlist index (stores IDs for songs in that playlist)
	string tracklistIndexLocation_;
	//list of directories to search for files in
	vector<string> directories_;
	//for ID management to prevent misallocation
	vector<int> unusedID_;
	int lastID_;

public:
	//constructor
	Index();
	//destructor
	~Index();

	//Handles saving and loading config data for this class
	void InitiateIndexFromConfig();
	void WriteFieldsToConfig();

	//Handles directories_ management
	void AddDirectory(string newDirectory) { directories_.push_back(newDirectory); }
	string GetDirectory(int index) { return directories_.at(index); }
	int GetDirectorySize() { return directories_.size(); }
	void DeleteDirectory(string rmDirectory);

	//Reading from indices
	TrackList ReadMainIndex();
	vector<TrackList> ReadTrackListIndex();

	//Writing to indices
	void WriteTrackListIndex(vector<TrackList> in);
	void WriteTrackIndex(TrackList main);
	void UpdateTrackIndex();

	//Tools I made to help
	vector<string> GetAllEntries();

	bool isInVector(string input, vector<string> vector);

	TrackList TrackListFromIDList(vector<int> idList, string name);

	int NewUniqueId();


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