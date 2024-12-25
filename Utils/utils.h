#ifndef UTILS_H
#define UTILS_H
#include <string>
using namespace std;
struct Song
{
    string title;
    int duration;
    string genre;
    string artist;
    string fileName;
};
void loadLibraryFromCSV(const string &fileName);
#endif