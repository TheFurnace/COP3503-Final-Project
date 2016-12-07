#include "Index.h"
#include <boost/filesystem.hpp>
namespace tl = TagLib;
namespace fs = boost::filesystem;

Index::Index()
	: ext_(".mp3"), trackIndexLocation_("trackIndex.txt"), tracklistIndexLocation_("trackListIndex.txt"), lastID_(0)
{
	InitiateIndexFromConfig();
	UpdateTrackIndex();
	ReadTrackListIndex();
}

Index::~Index()
{
	WriteFieldsToConfig();
	WriteTrackIndex();
	WriteTrackListIndex();
}

void Index::InitiateIndexFromConfig()
{
	ifstream config(CONFIG_FILE);
	string currentLine = "";

	if (!config.good())
		return;

	//define the easy ones
	getline(config, trackIndexLocation_);
	getline(config, tracklistIndexLocation_);
	string strLastID = "";
	getline(config, strLastID);
	lastID_ = stoi(strLastID);

	bool isDirectory = false;
	bool isUnusedID = false;

	//define the vectors
	while (getline(config, currentLine))
	{
		if (currentLine.compare("directories_") == 0)
			isDirectory = true;
		else if (currentLine.compare("/directories_") == 0)
			isDirectory = false;

		else if (currentLine.compare("unusedID_") == 0)
			isUnusedID = true;
		else if (currentLine.compare("/unusedID_") == 0)
			isUnusedID = false;

		else if (isDirectory)
			directories_.push_back(currentLine);
		else if (isUnusedID)
			unusedID_.push_back(stoi(currentLine));


	}
}

void Index::WriteFieldsToConfig()
{
	remove(CONFIG_FILE);
	ofstream config(CONFIG_FILE, fstream::out);

	config << trackIndexLocation_ << endl;
	config << tracklistIndexLocation_ << endl;
	config << lastID_ << endl;

	//write out directories
	config << "directories_" << endl;
	for (string &directory : directories_)
	{
		config << directory << endl;
	}
	config << "/directories_" << endl;

	//write out unused IDs
	config << "unusedID_" << endl;
	for (int &iD : unusedID_)
	{
		config << iD << endl;
	}
	config << "/unusedID_" << endl;

}

void Index::DeleteDirectory(string rmDirectory)
{
	for (unsigned int i = 0; i < directories_.size(); i++)
	{
		if (directories_[i].compare(rmDirectory) == 0)
		{
			directories_.erase(directories_.begin() + i);
		}
	}
}

void Index::ReadMainIndex()
{
	ifstream trackListIndexFile(tracklistIndexLocation_.c_str());

	TrackList newMain("main");
	string currentMetadata[METADATA_SIZE];
	string currentLine = "";
	int currentMetadataIndex = 0;
	bool isTrackInfo = false;

	while (getline(trackListIndexFile, currentLine))
	{
		if (currentLine.compare("Track") == 0)
			isTrackInfo = true;
		else if (currentLine.compare("/Track") == 0)
		{
			isTrackInfo = false;
			newMain.AddTrack(new Track(currentMetadata));
			currentMetadataIndex = 0;
		}
		else if (isTrackInfo && currentMetadataIndex < METADATA_SIZE)
		{
			currentMetadata[currentMetadataIndex++] = currentLine;
		}
	}

	main_ = new TrackList(newMain);
}

void Index::ReadTrackListIndex()
{
	vector<int> iDList;
	string currentName = "";

	ifstream trackListIndexFile(tracklistIndexLocation_.c_str());

	string currentLine = "";
	string currentToken = "";
	bool isTrackID = false;

	//Run as long as there is another line
	while (getline(trackListIndexFile, currentLine))
	{
		//If the line is a match, flag all following lines as track IDs and add them to a fresh vector
		if (currentLine.substr(0, 9) == "TrackList")
		{
			currentName = currentLine.substr(10, (int)currentLine.length());
			isTrackID = true;
			iDList.clear();
		}
		//If the line is a match, stop flagging all lines as track IDs, use the collected Ids to make a TrackList
		else if (currentLine.compare("/TrackList") == 0)
		{
			isTrackID = false;
			playLists_.push_back(TrackListFromIDList(iDList, currentName));
		}
		//If the line is flagged as a track ID, add it to the id list
		else if (isTrackID)
		{
			iDList.push_back(stoi(currentLine));
		}

	}
}

void Index::WriteTrackListIndex()
{
	remove(tracklistIndexLocation_.c_str());
	ofstream trackListIndexFile(tracklistIndexLocation_.c_str(), fstream::out);

	Track* currentTrack = nullptr;

	for (TrackList &currentList : playLists_)
	{
		trackListIndexFile << "TrackList " << currentList.name << endl;

		for (int i = 0; i < currentList.Size(); i++)
		{
			currentTrack = currentList.GetTrack(i);
			trackListIndexFile << currentTrack->getTrackID() << endl;
		}

		trackListIndexFile << "/TrackList" << endl;
	}
}

void Index::WriteTrackIndex()
{
	remove(trackIndexLocation_.c_str());
	ofstream trackListIndexFile(trackIndexLocation_.c_str(), fstream::out);

	Track* currentTrack = nullptr;

	for (int i = 0; i < main_->Size(); i++)
	{
		currentTrack = main_->GetTrack(i);

		trackListIndexFile << "Track" << endl;

		for (int j = 0; j < METADATA_SIZE; j++)
		{
			trackListIndexFile << currentTrack->getMetadata(j) << endl;
		}

		trackListIndexFile << "/Track" << endl;
	}


}

//Write a trackList file in this format, use NewUniqueID to make the ID for each track
/*
Track
(ID)
(Path)
(Title)
(Album)
(Artist)
(Year)
/Track
*/
void Index::UpdateTrackIndex()
{
	MetadataWorker worker;

	ifstream test(trackIndexLocation_.c_str());
	if (!test.good())
	{

		ofstream trackListIndexFile(trackIndexLocation_.c_str(), fstream::out);

		for (string path : GetAllEntries())
		{
			worker.SetFileDir(path);

			trackListIndexFile << "Track" << endl;

			trackListIndexFile << NewUniqueId() << endl;
			trackListIndexFile << path << endl;
			trackListIndexFile << worker.GetTrackNum() << endl;
			trackListIndexFile << worker.GetTitle() << endl;
			trackListIndexFile << worker.GetAlbum() << endl;
			trackListIndexFile << worker.GetArtist() << endl;
			trackListIndexFile << worker.GetYear() << endl;
			trackListIndexFile << worker.GetTrackLength() << endl;

			trackListIndexFile << "/Track" << endl;
		}
	}

	ReadTrackListIndex();
}

vector<string> Index::GetAllEntries()
{
	vector<string> allEntries;


	for (string& directory : directories_)
	{
		const fs::path root(directory);

		if (!fs::exists(root) || !fs::is_directory(root));
		else
		{
			fs::recursive_directory_iterator it(root);
			fs::recursive_directory_iterator endit;



			for (fs::directory_entry& entry : it)
			{
				if (fs::is_regular_file(entry) && it->path().extension() == ext_)
				{
					if (!isInVector(entry.path().string(), allEntries))
						allEntries.push_back(entry.path().string());
				}
			}
		}
	}

	return allEntries;
}

TrackList Index::TrackListFromIDList(vector<int> idList, string name)
{
	ifstream in(tracklistIndexLocation_);
	string currentString = "";
	TrackList out(name);
	string metadataArr[METADATA_SIZE];
	int index = 0;
	bool match = false;

	for (int id : idList)
	{
		index = 0;
		while (getline(in, currentString))
		{
			if (currentString.compare(to_string(id)) == 0)
			{
				match = true;
				metadataArr[index++] = currentString;
			}
			else if (currentString.compare("/Track") == 0)
				match = false;
			else if (match == true)
			{
				metadataArr[index++] = currentString;
			}
		}

		out.AddTrack(new Track(metadataArr));
	}

	return out;
}

int Index::NewUniqueId()
{
	int id;
	if (unusedID_.size() > 0)
	{
		id = unusedID_.front();
		unusedID_.erase(unusedID_.begin(), unusedID_.begin() + 1);
	}
	else
		id = lastID_ + 1;

	lastID_ = id;

	return id;
}

bool Index::isInVector(string input, vector<string> vector)
{
	for (string entry : vector)
	{
		if (input.compare(entry) == 0)
			return true;
	}
	return false;
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
}

int MetadataWorker::GetTrackLength()
{
	TagLib::FileRef f(filedir.c_str());

	if (!f.isNull() && f.tag()) {

		return f.audioProperties()->lengthInSeconds();
	}
}

