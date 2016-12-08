#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

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
  
  
  while(true)
    {
      cout << ">>";
      string input;
      cin >> input;

      if(input == "play") { // Use boost::iequals
        cout << "Play track\n";
        //} else if (input == "help") { cout << helpText;
      } else if (input == "pause") {
        cout << "Pause track\n";
      } else if (input == "next") {
        cout << "Next track\n";
      } else if (input == "previous") {
        cout << "Previous track\n";
      } else if (input == "mkplaylist") {
        cout << "Input playlist name: ";
        cin >> input;
        cout << "Create playlist " << input << "\n";
      } else if (input == "rmplaylist") {
        cout << "Input playlist name: ";
        cin >> input;
        cout << "Remove playlist " << input << "\n";
      } else if (input == "displaylist") {
        cout << "Input playlist name: ";
        cin >> input;
        cout << "Display playlist " << input << "\n";
      } else if (input == "manageplaylist") {
        cout << "Input playlist name: ";
        cin >> input;
        //manageplaylist(input);
      } else if (input == "managetrack") {
        cout << "Input track unique ID or \"current\" to manage current track: ";
        cin >> input;
        cout << "Managing " << input << " track.";
        //managetrack(
      } else if (input == "search") {
        cout << "Input Metadata tag to search for: ";
        cin >> input;
      } else if (input == "nowplaying") {
        cout << "[ID] Title - Album by Artist (0:00)\n";
      } else if (input == "exit") { break;
      } else {
        cout << "Unknown option, type \"help\" for help\n";
      }
    }
  return 0;
}

int main(int argc, char * argv[])
{
  if(cmdOptionExists(argv, argv+argc, "-h"))
    {
      cout << "help text" << "\n";
    }

  char * filename = getCmdOption(argv, argv + argc, "-f");

  if (filename)
    {
      cout << "add " << filename << " to tracklist\n";
    }

  startTBI();

  return 0;
}
