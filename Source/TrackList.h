#pragma once
//This is a symbolic global variable, it's a value that will be constant and so wont take up ram. Also remember to update it if you add something to the metadata array
#define METADATA_SIZE 8
#include <string>
using std::string;

class Track
{
	//this is an array of the actual metadata
	string* metadataArr_ = nullptr;
	//this is an array of labels for every metadata entry, it is static and constant and don't touch it. I only barely understand why it is the way it is.
	//It is initialized in the cpp but declared in the header
	static const string metadataLabelArr_[METADATA_SIZE];

	//internal methods
	static int indexOfThisMetadata(string metadataLabel);
	string labelOfThisIndex(int index);

public:
	Track() :metadataArr_(new string[METADATA_SIZE]) { /* literally nothing  */ }
	Track(string newMetadataArr[METADATA_SIZE]);

	//returns metadata by label
	string getMetadata(string metadataLabel);
	//returns metadata by index
	string getMetadata(int i) { return metadataArr_[i];  }
	//returns path of track
	string getPath() { return metadataArr_[1]; }
	//changes a piece of metadata per label
	void setMetadata(string metadataLabel, string newData);
	//changes a piece of metadata per index
	void setMetadata(int i, string input) { metadataArr_[i] = input; }
	
	//Getter and setter for track ID
	int getTrackID() { return stoi(metadataArr_[0]); }
	void setTrackID(int trackID) { metadataArr_[0] = trackID; }

	//for testing methods but can be useful
	int metadataSize() { return METADATA_SIZE; }

};

//A structure is a class with default public fields. It is used for classes that would only hold data without having any functions
struct Node
{
	//Each node holds a track pointer and a node pointer
	Track* data;
	Node* next;

	Node(Track* newData);
	Node(Track* newData, Node* nextNode);

};

class TrackList
{
	Node* first_;
	Node* last_;
	string name_;
	int size_;

public:
	TrackList(string newName);
	TrackList(string newName, Track* newTrack);

	//standard linked list stuff
	void AddTrack(Track* newTrack);
	void AddTrack(Track* newTrack, int index);
	void RemoveTrack(int index);
	Track* GetTrack(int index);
	int Size() { return size_; }


	//Search for string matches in tag
	TrackList search(string tag, string searchToken);
	//return string
	string String();
};