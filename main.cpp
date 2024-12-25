#include <iostream>
#include <raylib.h>
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

    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Music Library");

    const int fontSize = 20;
    Color textColor = WHITE;

    char titleInput[100] = {0};  // Buffer for song title input
    char artistInput[100] = {0}; // Buffer for artist input

    // initialize stuff for raylib

    int choice = 0;
    string title;
    string artist;
    LibraryManager libraryManager;
    libraryManager.loadLibraryFromCSV("Database/library.csv");

    while (!WindowShouldClose())
    {

        // start drwaing and clear screen
        BeginDrawing();
        ClearBackground(BLACK);

        // display text

        DrawText("Menu:", 20, 20, fontSize, textColor);
        DrawText("| 1. Display Music Library. |", 120, 20, fontSize, textColor);
        DrawText("2. Search By Title. |", 400, 20, fontSize, textColor);
        DrawText("3. Search By Artist. |", 620, 20, fontSize, textColor);
        DrawText("4. Exit |", 850, 20, fontSize, textColor);

        // prompt

        DrawText("Enter your choice (1-4): ", 20, 60, fontSize, textColor);

        if (IsKeyPressed(KEY_ONE))
        {
            choice = 1;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            choice = 2;
        }
        if (IsKeyPressed(KEY_THREE))
        {
            choice = 3;
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            choice = 4;
        }

        // cout << "Menu:" << endl;
        // cout << "1. Display Music Library." << endl;
        // cout << "2. Search By Title." << endl;
        // cout << "3. Search By Artist." << endl;
        // cout << "4. Exit" << endl;
        // cout << "Enter your choice (1-4): ";

        // cin >> choice;
        switch (choice)
        {
        case 1:
            libraryManager.display();
            DrawText("Displaying Music Library", 20, 260, fontSize, textColor);
            break;
        case 2:
            DrawText("Enter the title of the song you want to search: ", 20, 260, fontSize, textColor);
            // cin >> title;
            libraryManager.searchByTitle(titleInput);
            DrawText("Searching By Title", 20, 300, fontSize, textColor);
            break;
        case 3:
            // cout << "Enter the artist of the song you want to search: ";
            DrawText("Enter the artist of the song you want to search: ", 20, 260, fontSize, textColor);
            // cin >> artist;

            libraryManager.searchByArtist(artistInput);
            DrawText("Searching By Artist", 20, 300, fontSize, textColor);
            break;
        case 4:
            // cout << "Exiting the program..." << endl;
            DrawText("Exiting the program...", 20, 260, fontSize, textColor);
            EndDrawing();
            return 0;
        default:
            // cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            DrawText("Invalid choice. Please enter a number between 1 and 4.", 20, 260, fontSize, textColor);
        }

        EndDrawing();
    }
    CloseWindow();
}