#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <Mmsystem.h>
#include <iomanip>
#include <stdio.h>
#include <mciapi.h>
#include <boost/filesystem.hpp>

#include <tag.h>
#include <tpropertymap.h>
#include <fileref.h>

using namespace std;
#pragma comment(lib, "Winmm.lib")

#include "..\..\Source\TrackList.cpp"
#include "..\..\Source\Index.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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


namespace TrackListTest
{
	TEST_CLASS(TrackListTesting)
	{
	public:

		//Example naming convention for a test method
		TEST_METHOD(SubjectOfTest_DescriptionOfTest_PredictedOutcomeOfTest)
		{
			// TODO: Your test code here
			Assert::IsTrue(1 == 1);
		}

		TEST_METHOD(Constructor_InstantiateWithNullTrack_DataEqualsInput)
		{
			Track* testTrack = (Track*)5;
			TrackList testList("test", testTrack);

			Assert::IsTrue(testList.GetTrack(0) == testTrack);
		}

		TEST_METHOD(AddTrack_AddNullTrack_DataEqualsInput)
		{
			Track* testTrack = (Track*)5;
			TrackList testList = TrackList("test");
			testList.AddTrack(testTrack);

			Assert::IsTrue(testList.GetTrack(0) == testTrack);
		}

		TEST_METHOD(AddTrack_AddManyNullTracks_DataEqualsInput)
		{
			Track* testTrack = (Track*)5;
			TrackList testList = TrackList("test");
			testList.AddTrack(testTrack, 0);
			testList.AddTrack(testTrack, 1);
			testList.AddTrack(testTrack, 2);


			Assert::IsTrue(testList.GetTrack(2) == testTrack);
		}

		TEST_METHOD(AddTrack_AddNullTracksWithDifferentValues_DataEqualsInput)
		{
			Track* testTrack = (Track*)5;
			Track* pseudoTestTrack = (Track*)6;
			TrackList testList = TrackList("test");
			testList.AddTrack(pseudoTestTrack);
			testList.AddTrack(pseudoTestTrack);
			testList.AddTrack(pseudoTestTrack);

			testList.AddTrack(testTrack, 1);

			Assert::IsTrue(testList.GetTrack(1) == testTrack);
		}

		TEST_METHOD(GetTrack_AddATonOfTracks_DataIsReturnedQuickly)
		{
			Track* testTrack = (Track*)5;
			TrackList testList = TrackList("test");

			for (int i = 0; i < 10000; i++)
			{
				testList.AddTrack(testTrack);
			}

			Assert::IsTrue(testList.GetTrack(9999) == testTrack);
		}

		TEST_METHOD(RemoveTrack_RemoveTrackChangesValueToNextTrack_DataEqualsInput)
		{
			Track* testTrack = (Track*)5;
			Track* pseudoTestTrack = (Track*)6;
			TrackList testList = TrackList("test");
			testList.AddTrack(pseudoTestTrack);
			testList.AddTrack(pseudoTestTrack);
			testList.AddTrack(testTrack);
			testList.AddTrack(pseudoTestTrack);
			testList.AddTrack(pseudoTestTrack);

			Assert::IsTrue(testList.GetTrack(2) == testTrack);
			testList.RemoveTrack(2);
			Assert::IsTrue(testList.GetTrack(2) == pseudoTestTrack);
		}


	};

	TEST_CLASS(TrackTesting)
	{
	public:
		TEST_METHOD(Constructor_NewTrackFromMetadata_MetadataIsStored)
		{
			string metadataArr[5] = { "1","2","3","4","5" };
			Track testTrack = Track(metadataArr);

			Assert::AreEqual(testTrack.getMetadata("id"), string("1"));
			Assert::AreEqual(testTrack.getMetadata("title"), string("3"));
		}

		TEST_METHOD(SetMetadata_ChangeMetadata_MetadataIsChanged)
		{
			string metadataArr[METADATA_SIZE] = { "1","2","3","4","5", "6" };
			Track testTrack = Track(metadataArr);

			Assert::AreEqual(testTrack.getMetadata("path"), string("2"));

			testTrack.setMetadata("path", "7");

			Assert::AreEqual(testTrack.getMetadata("path"), string("7"));
		}


	};

	TEST_CLASS(MusicPlayerTesting)
	{
	public:
		TEST_METHOD(SlackPostedCode_MakesSound)
		{
			//L"open \" + file path and name + \"....
			string locationOfMusic = "C:\\Users\\Centurion\\Desktop\\test\\Album.mp3";

			ostringstream os;
			os << "open \"" << locationOfMusic << "\" type MPEGvideo alias song";

			wstring fullInput = s2ws(os.str());
			//On my computer i only need mscisendstring but on the lab comps i needed to add the "W". This might need to be changed depending on whos comp we demo on.
			LPCWSTR a = fullInput.c_str();
			mciSendStringW(a, NULL, 0, NULL);
			//PLAY
			mciSendString("play song", NULL, 0, NULL);


		}
	};

	TEST_CLASS(IndexTesting)
	{
	public:
		TEST_METHOD(IndexConstructor_CanInitiate_NoError)
		{
			Index testIndex;
		}

		TEST_METHOD(AddDirectory_CanAddDirectory_DirectoryIsSameAsInputAndSizeGoesUp)
		{
			remove(CONFIG_FILE);

			Index testIndex;
			testIndex.AddDirectory("test0");
			testIndex.AddDirectory("test1");
			testIndex.AddDirectory("test2");

			//Assert::AreEqual(string("test1"), testIndex.GetDirectory(1));
			Assert::IsTrue(testIndex.GetDirectorySize() == 3);

		}

		TEST_METHOD(DeleteDirectory_DirectoryIsSameAsThirdInputAndSizeGoesDown)
		{
			remove(CONFIG_FILE);

			Index testIndex;
			testIndex.AddDirectory("test0");
			testIndex.AddDirectory("test1");
			testIndex.AddDirectory("test2");

			testIndex.DeleteDirectory("test1");

			Assert::AreEqual(string("test2"), testIndex.GetDirectory(1));
			Assert::IsTrue(testIndex.GetDirectorySize() == 2);
		}

		TEST_METHOD(WriteFieldsToConfig_CanWriteInitialState_NoError)
		{
			remove(CONFIG_FILE);

			Index testIndex;

			testIndex.AddDirectory("test0");
			testIndex.AddDirectory("test1");
			testIndex.AddDirectory("test2");
		}

		TEST_METHOD(InitiateIndexFromConfig_CanReadInitialState_InititatesAccordingToFile)
		{
			remove(CONFIG_FILE);

			if (true)
			{
				Index testIndex;

				testIndex.AddDirectory("test0");
				testIndex.AddDirectory("test1");
				testIndex.AddDirectory("test2");
			}

			Index testIndex;

			Assert::AreEqual(string("test1"), testIndex.GetDirectory(1));
			Assert::IsTrue(testIndex.GetDirectorySize() == 3);

		}

		TEST_METHOD(WriteTrackList_CanWriteToTextFile_NoError)
		{
			remove(CONFIG_FILE);

			Index testIndex;

			vector<TrackList> in;
			in.push_back(TrackList("test0"));
			in.push_back(TrackList("test1"));
			in.push_back(TrackList("test2"));

			testIndex.WriteTrackListIndex(in);
		}

		TEST_METHOD(ReadTrackList_CanReadFromTextFile_IsCorrect)
		{
			remove(CONFIG_FILE);

			Index testIndex;

			vector<TrackList> in;
			in.push_back(TrackList("test0"));
			in.push_back(TrackList("test1"));
			in.push_back(TrackList("test2"));

			testIndex.WriteTrackListIndex(in);

			vector<TrackList> out = testIndex.ReadTrackListIndex();

			Assert::AreEqual(in[1].name, out[1].name);

		};

		TEST_METHOD(ReadDirectoryContents_CanReadFromGivenDirectories_AnyOutput)
		{
			Index testIndex;

			testIndex.AddDirectory("C:\\Users\\Centurion\\Desktop\\test");

			vector<string> mp3 = testIndex.GetAllEntries();

			Assert::IsTrue(mp3.size() == 1);
		};
	};

	TEST_CLASS(MetadataTesting)
	{
	public:
		TEST_METHOD(Metadata_Sandbox)
		{
			ostringstream out;
			string path = "C:\\Users\\Centurion\\Music\\Xbox Music\\Subscription Cache\\Ana Tijoux\\La Bala\\01 La Bala.wma";

			out << "******************** \"" << path << "\" ********************" << endl;

			TagLib::FileRef f(path.c_str());

			if (!f.isNull() && f.tag()) {

				TagLib::Tag *tag = f.tag();

				out << "-- TAG (basic) --" << endl;
				out << "title   - \"" << tag->title() << "\"" << endl;
				out << "artist  - \"" << tag->artist() << "\"" << endl;
				out << "album   - \"" << tag->album() << "\"" << endl;
				out << "year    - \"" << tag->year() << "\"" << endl;
				out << "comment - \"" << tag->comment() << "\"" << endl;
				out << "track   - \"" << tag->track() << "\"" << endl;
				out << "genre   - \"" << tag->genre() << "\"" << endl;

				TagLib::PropertyMap tags = f.file()->properties();

				unsigned int longest = 0;
				for (TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
					if (i->first.size() > longest) {
						longest = i->first.size();
					}
				}

				out << "-- TAG (properties) --" << endl;
				for (TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
					for (TagLib::StringList::ConstIterator j = i->second.begin(); j != i->second.end(); ++j) {
						out << left << std::setw(longest) << i->first << " - " << '"' << *j << '"' << endl;
					}
				}

			}

			if (!f.isNull() && f.audioProperties()) {

				TagLib::AudioProperties *properties = f.audioProperties();

				int seconds = properties->length() % 60;
				int minutes = (properties->length() - seconds) / 60;

				out << "-- AUDIO --" << endl;
				out << "bitrate     - " << properties->bitrate() << endl;
				out << "sample rate - " << properties->sampleRate() << endl;
				out << "channels    - " << properties->channels() << endl;
				out << "length      - " << minutes << ":" << setfill('0') << setw(2) << seconds << endl;

				Logger::WriteMessage(out.str().c_str());
			}
		}

	};
}