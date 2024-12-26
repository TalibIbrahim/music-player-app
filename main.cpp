#include <iostream>
// #include <raylib.h>
#include <fstream>
#include <sstream>
using namespace std;

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
class LibraryManager
{
private:
    Library *libraryHead;

public:
    LibraryManager()
    {
        libraryHead = NULL;
    }
    ~LibraryManager()
    {
        // Library *temp = libraryHead;
        // while (temp != NULL)
        // {
        //     Library *next = temp->next;
        //     delete temp;
        //     temp = next;
        // }
    }
    void addToLibrary(Song song)
    {
        Library *newSong = new Library();
        newSong->data = song;
        newSong->next = NULL;
        newSong->prev = NULL;
        if (libraryHead == NULL)
        {
            libraryHead = newSong;
        }
        else
        {
            Library *temp = libraryHead;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newSong;
            newSong->prev = temp;
        }
    }
    void display()
    {
        Library *temp = libraryHead;

        while (temp != NULL)
        {
            cout << temp->data.title << " ";
            cout << temp->data.duration << " ";
            cout << temp->data.genre << " ";
            cout << temp->data.artist << " ";
            cout << endl;
            temp = temp->next;
        }
    }
    void searchByTitle(string title)
    {
        Library *temp = libraryHead;
        while (temp != NULL)
        {
            if (temp->data.title == title)
            {
                cout << temp->data.title << " ";
                cout << temp->data.duration << " ";
                cout << temp->data.genre << " ";
                cout << temp->data.artist << " ";
                cout << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Song not found" << endl;
    }
    void searchByArtist(string artist)
    {
        Library *temp = libraryHead;

        while (temp != NULL)
        {

            if (temp->data.artist == artist)
            {

                cout << temp->data.title << " ";
                cout << temp->data.duration << " ";
                cout << temp->data.genre << " ";
                cout << temp->data.artist << " ";
                cout << endl;
            }
            temp = temp->next;
        }
    }
    void loadLibraryFromCSV(const string &fileName)
    {
        ifstream file(fileName); // Open the CSV file
        if (!file.is_open())
        {
            // Check if the file opened successfully
            cerr << "Error: Could not open file " << fileName << endl;
            return;
        }

        string line;
        getline(file, line); // Read and skip the header line

        // Read each remaining line of the CSV file
        while (getline(file, line))
        {
            stringstream ss(line); // Create a stringstream to parse the line
            string title, durationStr, genre, artist, fileName;

            // Extract each field from the line, separated by commas
            getline(ss, title, ',');       // Get the title
            getline(ss, durationStr, ','); // Get the duration (as a string)
            getline(ss, genre, ',');       // Get the genre
            getline(ss, artist, ',');      // Get the artist
            getline(ss, fileName, ',');    // Get the file name

            // Convert the duration from string to integer
            int duration = stoi(durationStr);

            // Create a Song object with the extracted data
            Song song = {title, duration, genre, artist, fileName};

            // Add the song to the library linked list
            addToLibrary(song);
        }

        file.close(); // Close the file after reading
    }
};
int main()
{

    int choice = 0;
    string title;
    string artist;
    LibraryManager libraryManager;
    libraryManager.loadLibraryFromCSV("Database/library.csv");

    while (true)
    {
        // Display menu options
        cout << "Menu:" << endl;
        cout << "1. Display Music Library." << endl;
        cout << "2. Search By Title." << endl;
        cout << "3. Search By Artist." << endl;
        cout << "4. Exit" << endl;

        // Get user's choice
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        cin.ignore(); // Ignore the newline character left by cin

        switch (choice)
        {
        case 1:
            libraryManager.display();
            break;
        case 2:
            cout << "Enter Title: ";
            cin >> title;
            libraryManager.searchByTitle(title);
            break;
        case 3:
            cout << "Enter Artist: ";
            cin >> artist;
            libraryManager.searchByArtist(artist);
            break;
        case 4:
            cout << "Exiting the program..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            break;
        }
    }

    return 0;
}