// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include "utils.h"

// extern LibraryNode *head;

// void loadLibraryFromCSV(const std::string &fileName)
// {
//     std::ifstream file(fileName);
//     if (!file.is_open())
//     {
//         std::cerr << "Error: Could not open file " << fileName << std::endl;
//         return;
//     }

//     std::string line;
//     std::getline(file, line);

//     while (std::getline(file, line))
//     {
//         std::stringstream ss(line);
//         std::string title, durationStr, genre, artist, fileName;
//         std::getline(ss, title, ',');
//         std::getline(ss, durationStr, ',');
//         std::getline(ss, genre, ',');
//         std::getline(ss, artist, ',');
//         std::getline(ss, fileName, ',');

//         int duration = std::stoi(durationStr);

//         Song song = {title, duration, genre, artist, fileName};

//         addSong(song);
//     }

//     file.close();
// }