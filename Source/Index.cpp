#include "Index.h"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

Index::Index()
	: ext(".mp3"), trackIndexLocation_("trackIndex.txt"), tracklistIndexLocation_("trackListIndex.txt"), lastID_(0)
{
	InitiateIndexFromConfig();
}

Index::~Index()
{
	WriteFieldsToConfig();
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

TrackList * Index::ReadMainIndex()
{
	return nullptr;
}

vector<TrackList> Index::ReadTrackListIndex()
{
	vector<TrackList> currentList;
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
			currentList.push_back(TrackListFromIDList(iDList, currentName));
		}
		//If the line is flagged as a track ID, add it to the id list
		else if (isTrackID)
		{
			iDList.push_back(stoi(currentLine));
		}

	}

	return currentList;
}

void Index::WriteTrackListIndex(vector<TrackList> tListV)
{
	remove(tracklistIndexLocation_.c_str());
	ofstream trackListIndexFile(tracklistIndexLocation_.c_str(), fstream::out);

	Track* currentTrack = nullptr;

	for (TrackList &currentList : tListV)
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

}

void Index::UpdateTrackIndex()
{
	ifstream inDex(trackIndexLocation_);

	if (!inDex.good())
	{
		WriteTrackIndex();
		return;
	}



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
				if (fs::is_regular_file(entry) && it->path().extension() == ext)
				{
					if(!isInVector(entry.path().string(), allEntries))
						allEntries.push_back(entry.path().string());
				}
			}
		}
	}

	return allEntries;
}

TrackList Index::TrackListFromIDList(vector<int> idList, string name)
{
	//This is a temporary placeholder to test methods that rely on this method
	return TrackList(name);
}

bool Index::isInVector(string input, vector<string> vector)
{
	for (string entry : vector)
	{
		if (input.compare(entry)==0)
			return true;
	}
	return false;
}
