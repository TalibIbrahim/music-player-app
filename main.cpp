#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;
string toLower(string &str)
{
    // convert to lowercase including spaces and return a copy of original string
    string newStr = str;
    for (int i = 0; i < newStr.length(); i++)
    {
        newStr[i] = tolower(newStr[i]);
    }
    return newStr;
}
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
struct Playlist
{
    string name;
    Library *songs;
    Playlist *next;
    Playlist *prev;
};

struct UserLibrary
{
    Playlist *playlistsHead;
    UserLibrary *next;
    UserLibrary *prev;
};

struct currentlyPlayingQueue
{
    Song data;
    currentlyPlayingQueue *next;
    currentlyPlayingQueue *prev;
};

currentlyPlayingQueue *currentlyPlayingHead = NULL;
currentlyPlayingQueue *currentlyPlayingSong = NULL;
currentlyPlayingQueue *currentlyPlayingTail = NULL;

// Global library head

Library *libraryHead;

// Global Function to search for a song by title

Song *searchByTitle(string title)
{
    Library *temp = libraryHead;
    string convertedTitle = toLower(title);

    while (temp != NULL)
    {
        if (toLower(temp->data.title).find(convertedTitle) != string::npos) // Partial match
        {
            return &temp->data; // Return first match
        }
        temp = temp->next;
    }
    return NULL;
}

class UserLibraryManager
{
private:
    UserLibrary *userLibraryHead;

public:
    UserLibraryManager()
    {
        userLibraryHead = NULL;
    }
    ~UserLibraryManager()
    {
        UserLibrary *temp = userLibraryHead;
        while (temp != NULL)
        {
            UserLibrary *next = temp->next;
            delete temp;
            temp = next;
        }
    }
    void createPlaylistInLibrary(string name)
    {
        Playlist *newPlaylist = new Playlist();
        newPlaylist->name = name;
        newPlaylist->songs = NULL;
        newPlaylist->next = NULL;
        newPlaylist->prev = NULL;
        if (userLibraryHead == NULL)
        {
            userLibraryHead = new UserLibrary();
            userLibraryHead->playlistsHead = newPlaylist;
        }
        else
        {
            UserLibrary *temp = userLibraryHead;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = new UserLibrary();
            temp->next->prev = temp;
            temp->next->playlistsHead = newPlaylist;
        }
    }
    Playlist *getPlaylist(string name)
    {
        UserLibrary *temp = userLibraryHead;
        string convertedName = toLower(name);
        while (temp != NULL)
        {
            Playlist *tempPlaylist = temp->playlistsHead;
            while (tempPlaylist != NULL)
            {
                string convertedPlaylistName = toLower(tempPlaylist->name);
                if (convertedPlaylistName.find(convertedName) != string::npos)
                {
                    return tempPlaylist;
                }
                tempPlaylist = tempPlaylist->next;
            }
            temp = temp->next;
        }
        return NULL;
    }
    void addSongToQueue(Song song)
    {
        if (currentlyPlayingHead == NULL)
        {
            currentlyPlayingHead = new currentlyPlayingQueue();
            currentlyPlayingHead->data = song;
            currentlyPlayingHead->next = NULL;
            currentlyPlayingHead->prev = NULL;
            currentlyPlayingTail = currentlyPlayingHead;
        }
        else
        {
            currentlyPlayingQueue *newSong = new currentlyPlayingQueue();
            newSong->data = song;
            newSong->next = NULL;
            newSong->prev = currentlyPlayingTail;
            currentlyPlayingTail->next = newSong;
            currentlyPlayingTail = newSong;
        }
    }
    void displayQueue()
    {
        if (currentlyPlayingHead == NULL)
        {
            cout << "No songs in queue" << endl;
            return;
        }
        currentlyPlayingQueue *temp = currentlyPlayingHead;
        while (temp != NULL)
        {
            // data = song
            cout << temp->data.title << " ";
            cout << temp->data.duration << " ";
            cout << temp->data.genre << " ";
            cout << temp->data.artist << " ";
            cout << endl;
            temp = temp->next;
        }
    }
    void emptyQueue()
    {
        currentlyPlayingQueue *temp = currentlyPlayingHead;
        while (temp != NULL)
        {
            currentlyPlayingQueue *next = temp->next;
            delete temp;
            temp = next;
        }
        currentlyPlayingHead = NULL;
        currentlyPlayingTail = NULL;
    }
    void playSongFromPlaylist()
    {
        string playlistName;
        string songTitle;
        cout << "Enter the playlist name: ";
        getline(cin, playlistName);
        Playlist *playlist = getPlaylist(playlistName);
        if (playlist == NULL)
        {
            cout << "Playlist not found" << endl;
            return;
        }
        emptyQueue();
        Library *temp = playlist->songs; // first song in playlist
        if (temp == NULL)
        {
            cout << "No songs in playlist" << endl;
            return;
        }
        while (temp != NULL)
        {
            addSongToQueue(temp->data);
            temp = temp->next;
        }
        currentlyPlayingSong = currentlyPlayingHead;
        int choice = 0;
        while (true)
        {
            cout << endl;
            cout << "-----------------------------" << endl;
            cout << "      " << currentlyPlayingSong->data.title << endl;
            cout << "      " << currentlyPlayingSong->data.artist << endl;
            cout << "           \u23EE \u23F8 \u23ED" << endl;
            cout << "-----------------------------" << endl;
            cout << "1. Next \u23ED" << endl;
            cout << "2. Previous \u23EE" << endl;
            cout << "3. Add to Queue." << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                if (currentlyPlayingSong->next != NULL)
                {
                    currentlyPlayingSong = currentlyPlayingSong->next;
                }
                else
                {
                    cout << endl;
                    cout << "End of playlist" << endl;
                }
                break;
            case 2:
                if (currentlyPlayingSong->prev != NULL)
                {
                    currentlyPlayingSong = currentlyPlayingSong->prev;
                }
                else
                {
                    cout << endl;
                    cout << "Start of playlist" << endl;
                }
                break;
            case 3:
            {
                cout << "Enter Song you want to add in Queue: ";
                getline(cin, songTitle);
                Song *song = searchByTitle(songTitle);
                if (song == NULL)
                {
                    cout << endl;
                    cout << "Song not found" << endl;
                }
                else
                {
                    addSongToQueue(*song);
                }
                break;
            }
            case 4:
                emptyQueue();
                cout << "Exiting the player..." << endl;
                return;
            default:
                cout << "Invalid choice" << endl;
                break;
            }
        }
    }
    void addSongToPlaylist(string playlistName, Song song)
    {
        Playlist *playlist = getPlaylist(playlistName);
        if (playlist == NULL)
        {
            cout << "Playlist not found" << endl;
            return;
        }
        Library *newSong = new Library();
        newSong->data = song;
        newSong->next = NULL;
        newSong->prev = NULL;
        if (playlist->songs == NULL)
        {
            playlist->songs = newSong;
        }
        else
        {
            Library *temp = playlist->songs;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newSong;
            newSong->prev = temp;
        }
        cout << song.title << " added to playlist: " << playlist->name << endl;
    }
    void displayUserLibrary()
    {
        // displaying playlists in library without songs
        UserLibrary *temp = userLibraryHead;
        if (temp == NULL)
        {
            cout << "No playlists found" << endl;
            return;
        }
        while (temp != NULL)
        {
            cout << "Playlist: " << temp->playlistsHead->name << endl;
            temp = temp->next;
        }
    }
    void displayPlaylist(string playlistName)
    {
        Playlist *playlist = getPlaylist(playlistName);
        if (playlist == NULL)
        {
            cout << "Playlist not found" << endl;
            return;
        }
        Library *temp = playlist->songs;
        if (temp == NULL)
        {
            cout << "No songs in playlist" << endl;
            return;
        }
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
};
class LibraryManager
{
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
    void addSongToLibrary(Song song)
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
    void displaySong(Song song)
    {
        cout << song.title << " ";
        cout << song.duration << " ";
        cout << song.genre << " ";
        cout << song.artist << " ";
        cout << endl;
    }

    void searchByArtist(string artist)
    {
        string ConvertedArtist = toLower(artist);
        Library *temp = libraryHead;
        int count = 0;

        while (temp != NULL)
        {
            if (toLower(temp->data.artist) == ConvertedArtist)
            {

                cout << temp->data.title << " ";
                cout << temp->data.duration << " ";
                cout << temp->data.genre << " ";
                cout << temp->data.artist << " ";
                cout << endl;
                count++;
            }
            temp = temp->next;
        }
        if (count == 0)
        {
            cout << "Artist not found" << endl;
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
            addSongToLibrary(song);
        }

        file.close(); // Close the file after reading
    }
};
int main()
{
    string playlistName;
    UserLibraryManager userLibraryManager;
    Song *song;
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
        cout << "4. Create Playlist." << endl;
        cout << "5. Display Playlist." << endl;
        cout << "6. Add Song to Playlist." << endl;
        cout << "7. Display User Library." << endl;
        cout << "8. Play Song From Playlist." << endl;
        cout << "9. Exit" << endl;

        // Get user's choice
        cout << "Enter your choice (1-9): ";
        cin >> choice;
        cin.ignore(); // Ignore the newline character left by cin

        switch (choice)
        {
        case 1:
            libraryManager.display();
            break;
        case 2:
            cout << "Enter Title: ";
            getline(cin, title);
            song = searchByTitle(title);
            if (song == NULL)
            {
                cout << "Song not found" << endl;
            }
            else
            {
                libraryManager.displaySong(*song);
            }
            break;
        case 3:
            cout << "Enter Artist: ";
            getline(cin, artist);
            libraryManager.searchByArtist(artist);
            break;
        case 4:
            cout << "Enter Playlist Name: ";
            getline(cin, playlistName);
            userLibraryManager.createPlaylistInLibrary(playlistName);
            break;
        case 5:
            cout << "Enter Playlist Name: ";
            getline(cin, playlistName);
            userLibraryManager.displayPlaylist(playlistName);
            break;
        case 6:
            cout << "Enter Playlist Name: ";
            getline(cin, playlistName);
            if (userLibraryManager.getPlaylist(playlistName) == NULL)
            {
                cout << "Playlist not found" << endl;
                break;
            }
            cout << "Enter Song Title: ";
            getline(cin, title);
            song = searchByTitle(title);
            if (song == NULL)
            {
                cout << "Song not found" << endl;
            }
            else
            {
                userLibraryManager.addSongToPlaylist(playlistName, *song);
            }
            break;
        case 7:
            userLibraryManager.displayUserLibrary();
            break;
        case 8:
            userLibraryManager.playSongFromPlaylist();
            break;
        case 9:
            cout << "Exiting the program..." << endl;
            return 0;

        default:
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            break;
        }
    }

    return 0;
}