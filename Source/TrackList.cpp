#include "TrackList.h"
#include <sstream>
using std::ostringstream;

const string Track::metadataLabelArr_[METADATA_SIZE] = { "id", "location", "tracknum", "title", "album", "artist", "year", "tracklength" };

Node::Node(Track * newData)
{
	data = newData;
	next = nullptr;
}

Node::Node(Track* newData, Node* nextNode)
{
	data = newData;
	next = nextNode;
}



Track::Track(string newMetadataArr[METADATA_SIZE])
{
	metadataArr_ = newMetadataArr;
}

string Track::getMetadata(string metadataLabel)
{
	int index = indexOfThisMetadata(metadataLabel);
	return metadataArr_[index];
}

void Track::setMetadata(string metadataLabel, string data)
{
	int index = indexOfThisMetadata(metadataLabel);
	metadataArr_[index] = data;
}

//This will take in a string and return the index of said metadata in the metadataArr
int Track::indexOfThisMetadata(string metadataLabel)
{
	//iterate through metadataTypeArr
	for (int i = 0; i <= METADATA_SIZE; i++)
	{
		if (metadataLabel.compare(metadataLabelArr_[i]) == 0)
			return i;
	}
	
	return -1; //return -1 so that if the function fails it can be easily identified
}

string Track::labelOfThisIndex(int index)
{
	return metadataLabelArr_[index];
}





TrackList::TrackList(string newName)
{
	name_ = newName;
	first_ = nullptr;
	last_ = nullptr;
	size_ = 0;
}

TrackList::TrackList(string newName, Track* newTrack)
{
	name_ = newName;
	first_ = new Node(newTrack);
	last_ = first_;
	size_ = 1;
}

//Addtrack method for adding a track to the end of the list
void TrackList::AddTrack(Track * newTrack)
{
	//Case 1: adding first track
	if (first_ == nullptr)
	{
		first_ = new Node(newTrack);
		last_ = first_;
	}
	//Case 2: adding last track
	else
	{
		last_->next = new Node(newTrack);
		last_ = last_->next;
	}

	size_++;
}

void TrackList::AddTrack(Track * newTrack, int index)
{
	//Make sure input is in acceptable bounds
	if (index < 0 || index > size_)
	{
		throw std::runtime_error("Index out of bound exception in TrackList::AddTrack");
	}
	//Case 1: Add track as new first
	else if (index == 0)
	{
		if (first_ == nullptr)
		{
			first_ = new Node(newTrack, first_);
			last_ = first_;
		}
		else
			first_ = new Node(newTrack, first_);
	}
	//Case 2: Add track to end
	else if (index == size_ && index != 0)
	{
		AddTrack(newTrack);
	}
	//Case 3: Index out of bounds exception
	else
	{
		Node* current = first_;

		//loop until arrive at index previous to insertion
		for (int i = 0; i < index - 1; i++)
		{
			current = current->next;
		}

		current->next = new Node(newTrack, current->next);
	}
	size_++;
}

//This is a list of tracks
void TrackList::RemoveTrack(int index)
{
	//Make sure input is in acceptable bounds
	if (index < 0 || index >= size_)
	{
		throw std::runtime_error("Index out of bound exception in TrackList::RemoveTrack");
	}
	else if (index == 0)
	{
		first_ = first_->next;
	}
	else
	{
		Node* current = first_;

		//loop until arrive at index previous to deletion index
		for (int i = 0; i < index - 1; i++)
		{
			current = current->next;
		}

		current->next = current->next->next;
	}
	size_--;
}

Track * TrackList::GetTrack(int index)
{
	if (index < 0 || index >= size_)
	{
		throw std::runtime_error("Index out of bound exception in TrackList::GetTrack");
	}
	else
	{
		Node* current = first_;

		//loop until arrive at index
		for (int i = 0; i < index; i++)
		{
			current = current->next;
		}
		
		return current->data;
	}
}

//Never make a searchList without deleting the previous one
TrackList TrackList::search(string tag, string searchToken)
{
	TrackList newList("search result");
	string tokenMatch = "";

	for (int i = 0; i < size_; i++)
	{
		tokenMatch = this->GetTrack(i)->getMetadata(tag);
		if (searchToken.compare(tokenMatch) == 0)
			newList.AddTrack(this->GetTrack(i));
	}
	return newList;
}

string TrackList::String()
{
	int seconds = NULL;
	int minutes = NULL;
	Track* currentTrack;
	ostringstream os;

	for (int i = 0; i < size_; i++)
	{
		currentTrack = this->GetTrack(i);

		//"[ID] Song Title - Album by Artist (Duration)"
		seconds = stoi(currentTrack->getMetadata("tracklength")) % 60;
		minutes = (stoi(currentTrack->getMetadata("tracklength")) - seconds) / 60;

		os << "[" << currentTrack->getMetadata("id") << "] ";
		os << currentTrack->getMetadata("title") << " - ";
		os << currentTrack->getMetadata("album") << " by ";
		os << currentTrack->getMetadata("artist");
		os << " (" << minutes << ":" << seconds << ")" << std::endl;

		return os.str();
	}
}