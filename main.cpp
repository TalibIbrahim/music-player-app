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
            // insert at tail
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
    void sortPlaylist(bool ascending = true, bool duration = true)
    {
        string plName;
        cout << "Enter the name of playlist you want to sort: ";
        cin >> plName;
        Playlist *playlist = getPlaylist(plName);
        Library *temp = playlist->songs;
        Library *temp2 = NULL;
        bool swapped;
        if (temp == NULL)
        {
            cout << "No songs in playlist" << endl;
            return;
        }
        do
        {
            swapped = false;
            temp = playlist->songs;
            while (temp->next != temp2)
            {
                if (ascending && duration) // ascending order by duration
                {
                    if (temp->data.duration > temp->next->data.duration)
                    {
                        Song tempData = temp->data;
                        temp->data = temp->next->data;
                        temp->next->data = tempData;
                        swapped = true;
                    }
                }
                else if (!ascending && duration) // descending order by duration
                {
                    if (temp->data.duration < temp->next->data.duration)
                    {
                        Song tempData = temp->data;
                        temp->data = temp->next->data;
                        temp->next->data = tempData;
                        swapped = true;
                    }
                }
                else if (ascending && !duration) // ascending order by title
                {
                    if (temp->data.title > temp->next->data.title)
                    {
                        Song tempData = temp->data;
                        temp->data = temp->next->data;
                        temp->next->data = tempData;
                        swapped = true;
                    }
                }
                else if (!ascending && !duration) // descending order by title
                {
                    if (temp->data.title < temp->next->data.title)
                    {
                        Song tempData = temp->data;
                        temp->data = temp->next->data;
                        temp->next->data = tempData;
                        swapped = true;
                    }
                }
                else
                {
                    cout << "Invalid choice" << endl;
                }
                temp = temp->next;
            }
            temp2 = temp;
        } while (swapped);
        cout << endl;
        displayPlaylist(plName);
        cout << endl;
    }
    void addSongToQueue(Song song)
    {
        if (currentlyPlayingHead == NULL)
        {
            // if queue empty create head
            currentlyPlayingHead = new currentlyPlayingQueue();
            currentlyPlayingHead->data = song;
            currentlyPlayingHead->next = NULL;
            currentlyPlayingHead->prev = NULL;
            currentlyPlayingTail = currentlyPlayingHead;
        }
        else
        {
            // insert at tail
            currentlyPlayingQueue *newSong = new currentlyPlayingQueue();
            newSong->data = song;
            newSong->next = NULL;
            newSong->prev = currentlyPlayingTail;
            currentlyPlayingTail->next = newSong;
            currentlyPlayingTail = newSong;
        }
        cout << endl;
        cout << '"' << song.title << '"' << " added to queue" << endl;
        cout << endl;
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
    void loopQueue(bool enable = true)
    {
        // doubly ll converted to circular ll
        // toggle loop func
        // loop the currently playing queue so it plays in cycle
        if (enable)
        {
            currentlyPlayingTail->next = currentlyPlayingHead;
            currentlyPlayingHead->prev = currentlyPlayingTail;
        }
        else
        {
            currentlyPlayingTail->next = NULL;
            currentlyPlayingHead->prev = NULL;
        }
    }
    void emptyQueue()
    {
        // alwasy run before creating queu
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
        cout << "Enter the playlist name: ";
        getline(cin, playlistName);
        Playlist *playlist = getPlaylist(playlistName);
        if (playlist == NULL)
        {
            cout << "Playlist not found" << endl;
            return;
        }
        emptyQueue();                    // empty q if playlist found
        Library *temp = playlist->songs; // first song in playlist
        if (temp == NULL)                // if pl has song
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
        cout << endl;
        cout << "-----------------------------" << endl;
        cout << "      " << currentlyPlayingSong->data.title << endl;
        cout << "      " << currentlyPlayingSong->data.artist << endl;
        cout << "           \u23EE \u23F8 \u23ED" << endl;
        cout << "-----------------------------" << endl;
        bool isPlaying = true;
        bool isLooped = false;

        while (true)
        {
            int choice;
            cout << "1. Previous \u23EE" << endl;
            cout << "2. Play/Pause" << endl;
            cout << "3. Next \u23ED" << endl;
            cout << "4. Add to Queue." << endl;
            cout << "5. Loop Currently Playing Queue" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                system("clear");
                if (currentlyPlayingSong->prev != NULL)
                {
                    currentlyPlayingSong = currentlyPlayingSong->prev; // Move to previous song
                }
                else if (isLooped)
                {
                    currentlyPlayingSong = currentlyPlayingTail; // Jump to the last song in the queue if loop is enabled
                }
                else
                {
                    cout << endl;
                    cout << "Start of playlist" << endl;
                }

                cout << endl;
                cout << "-----------------------------" << endl;
                cout << "      " << currentlyPlayingSong->data.title << endl;
                cout << "      " << currentlyPlayingSong->data.artist << endl;
                cout << "           \u23EE \u23F8 \u23ED";
                if (isLooped)
                {
                    cout << "   \U0001F501"; // Loop icon
                }
                cout << endl;
                cout << "-----------------------------" << endl;
                break;

            case 2:
                system("clear");

                isPlaying = !isPlaying; // toggle play puase bool
                cout << endl;
                cout << "-----------------------------" << endl;
                cout << "      " << currentlyPlayingSong->data.title << endl;
                cout << "      " << currentlyPlayingSong->data.artist << endl;
                cout << "         \u23EE "; // Previous

                // Play/Pause toggle
                if (isPlaying)
                {
                    cout << "\u23F8"; // Pause
                }
                else
                {
                    cout << "\u23F5"; // Play
                }

                cout << " \u23ED"; // Next

                // Show loop icon if looping is enabled
                if (isLooped)
                {
                    cout << "   \U0001F501"; // 🔁 Repeat Queue
                }

                cout << endl;
                cout << "-----------------------------" << endl;

                break;
            case 3:
                system("clear");
                if (currentlyPlayingSong->next != NULL)
                {
                    currentlyPlayingSong = currentlyPlayingSong->next;
                }
                else
                {
                    cout << endl;
                    cout << "End of playlist" << endl;
                }
                cout << endl;
                cout << "-----------------------------" << endl;
                cout << "      " << currentlyPlayingSong->data.title << endl;
                cout << "      " << currentlyPlayingSong->data.artist << endl;
                cout << "           \u23EE \u23F8 \u23ED";
                if (isLooped)
                {
                    cout << "   \U0001F501";
                }
                cout << endl;
                cout << "-----------------------------" << endl;
                break;
            case 4:
            {
                cin.ignore(); // ignore newline character left by cin
                string songTitle;
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
            case 5:
                system("clear");
                isLooped = !isLooped; // toggle loop
                loopQueue(isLooped);
                if (isLooped)
                {
                    cout << "Queue looped" << endl;
                }
                else
                {
                    cout << "Queue unlooped" << endl;
                }
                break;
            case 6:
                system("clear");
                loopQueue(false);
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
            // insert at tail
            Library *temp = playlist->songs;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newSong;
            newSong->prev = temp;
        }
        cout << endl;
        cout << '"' << song.title << '"' << " added to playlist: " << playlist->name << endl;
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
        Library *temp = libraryHead;
        while (temp != NULL)
        {
            Library *next = temp->next;
            delete temp;
            temp = next;
        }
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
        cout << endl;
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
    int choice = 0;
    string title;
    string artist;
    LibraryManager libraryManager;
    libraryManager.loadLibraryFromCSV("Database/library.csv");

    while (true)
    {
        // Display menu options
        cout << endl;
        cout << "** Menu **:" << endl;
        cout << "1. Display Music Library." << endl;
        cout << "2. Search By Title." << endl;
        cout << "3. Search By Artist." << endl;
        cout << "4. Create Playlist." << endl;
        cout << "5. Display Playlist." << endl;
        cout << "6. Add Song to Playlist." << endl;
        cout << "7. Sort Playlist." << endl;
        cout << "8. Display User Library." << endl;
        cout << "9. Play Song From Playlist." << endl;
        cout << "10. Exit" << endl;

        // Get user's choice
        cout << "Enter your choice (1-10): ";
        cin >> choice;
        cin.ignore(); // Ignore the newline character left by cin

        switch (choice)
        {
        case 1:
            system("clear");
            libraryManager.display();
            break;
        case 2:
        {
            system("clear");
            cout << "Enter Title: ";
            getline(cin, title);
            Song *song = searchByTitle(title);
            if (song == NULL)
            {
                cout << "Song not found" << endl;
            }
            else
            {
                libraryManager.displaySong(*song);
            }
            break;
        }
        case 3:
            system("clear");
            cout << "Enter Artist: ";
            getline(cin, artist);
            libraryManager.searchByArtist(artist);
            break;
        case 4:
            system("clear");
            cout << "Enter Playlist Name: ";
            getline(cin, playlistName);
            userLibraryManager.createPlaylistInLibrary(playlistName);
            break;
        case 5:
            system("clear");
            cout << "Enter Playlist Name: ";
            getline(cin, playlistName);
            userLibraryManager.displayPlaylist(playlistName);
            break;
        case 6:
        {
            system("clear");
            libraryManager.display();
            cout << "Enter Playlist Name: ";
            getline(cin, playlistName);
            if (userLibraryManager.getPlaylist(playlistName) == NULL)
            {
                cout << "Playlist not found" << endl;
                break;
            }
            cout << "Enter Song Title: ";
            getline(cin, title);
            Song *song = searchByTitle(title);
            if (song == NULL)
            {
                cout << "Song not found" << endl;
            }
            else
            {
                userLibraryManager.addSongToPlaylist(playlistName, *song);
            }
            break;
        }
        case 7:
            system("clear");
            int choice;
            cout << "1. Sort Playlist by Title (Ascending)" << endl;
            cout << "2. Sort Playlist by Title (Descending)" << endl;
            cout << "3. Sort Playlist by Duration (Ascending)" << endl;
            cout << "4. Sort Playlist by Duration (Descending)" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                userLibraryManager.sortPlaylist(true, false);
                cout << "*Playlist sorted by title in ascending order*" << endl;
                break;
            case 2:
                userLibraryManager.sortPlaylist(false, false);
                cout << "*Playlist sorted by title in descending order*" << endl;
                break;
            case 3:
                userLibraryManager.sortPlaylist(true, true);
                cout << "*Playlist sorted by duration in ascending order*" << endl;
                break;
            case 4:
                userLibraryManager.sortPlaylist(false, true);
                cout << "*Playlist sorted by duration in descending order*" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
                break;
            }
            break;
        case 8:
            system("clear");
            userLibraryManager.displayUserLibrary();
            break;
        case 9:
            system("clear");
            userLibraryManager.playSongFromPlaylist();
            break;
        case 10:
            cout << "Exiting the program..." << endl;
            return 0;

        default:
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            break;
        }
    }

    return 0;
}
// post code circlejerk session @bytercrew