#include <iostream>
#include <raylib.h>

using namespace std;

int main()
{

    struct Song
    {
        string title;
        int duration;
        string genre;
        string artist;
        string fileName; // for future when we add the option to play the mp3 files.
    };

    struct Library
    {
        Song data;
        Library *next;
        Library *prev;
    };
}