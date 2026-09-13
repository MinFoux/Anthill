//  Quick outline of goals for Anthill:
//  Memory of the last directory location picked by the user.
//  Awareness of the files (ants) the user opened in the most recent session.
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <list>
using namespace std;

string selectedAnt = "NONE", textbox = "", prefix = "Anthill>  ";
list<string> display = { "Welcome to Anthill!","Press l to display all Ants","Press f to focus selected Ant" };
int cursorFlashRate = 15000, cursorFlashCounter = 0;
bool cursorVisible = true;
char cursorChar = '_';


void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;

    COORD home = { 0, 0 };

    FillConsoleOutputCharacter(
        hConsole,
        ' ',
        cells,
        home,
        &written
    );

    FillConsoleOutputAttribute(
        hConsole,
        csbi.wAttributes,
        cells,
        home,
        &written
    );

    SetConsoleCursorPosition(hConsole, home);
}

void printScreen() {
	clearScreen();
	list<string> tDisplay = display;
    for (int i = 0; i < tDisplay.size(); i++) {
        cout << tDisplay.front() << "\n";
        tDisplay.pop_front();
    }
}

//Cursorflash Variables
auto lastCursorFlash = chrono::steady_clock::now();

void cursorFlash(int type) {
    printf("\x1B[38;5;255m");
    auto now = chrono::steady_clock::now();
    switch (type) {
        case 1:
            
            if (chrono::duration_cast<chrono::milliseconds>(now - lastCursorFlash).count() >= 500)
            {
                cursorVisible = !cursorVisible;
                lastCursorFlash = now;

                    printf("\033[1D");
                cout << (cursorVisible ? cursorChar : ' ') << flush;
            }
            break;

        case 2:
            if (chrono::duration_cast<chrono::milliseconds>(now - lastCursorFlash).count() >= 500)
            {
                cursorVisible = !cursorVisible;
                lastCursorFlash = now;
                cout << (cursorVisible ? cursorChar : ' ') << flush;
            }
            break;

        case 3:
            cout << (cursorVisible ? cursorChar : ' ') << flush;
            break;
    }
}


//The variables that are so important to the program that they need to be global. Apparently.
int main()
{
	//Force the very distracting cursor to stop blinking and hide it from the user.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);

    cursorInfo.bVisible = FALSE;

    SetConsoleCursorInfo(hConsole, &cursorInfo);

	//Clean up the screen and begin printing the interface loop.
    clearScreen();
    printf("\x1B[38;5;9m");
    printScreen();
    // print initial cursor and prefix
    cout << prefix;

    cursorFlash(3);
    
    while (true) {
        
        if (_kbhit())
        {
            char key = _getch();

            switch(key) {

                case '\b':
				    if (textbox != "") {
					    textbox.pop_back();
                        printf("\x1B[1D");
                        printf("\x1B[1D");
                        printf("\x1B[0J");
						cursorFlash(3);
				    }
				    break;

                case '\r':
                    display.push_front(prefix + textbox);
                    printf("\x1b[2K"); // Clear entire line
					printf("\x1B[0E"); // Move to beginning of next line
                    printf("\x1B[1F"); // Undo what I just did because nothing else worked and I'm so tired. I love c++!!!!!!
                    printf("\x1B[38;5;9m");
                    cout << prefix;
                    printf("\x1B[38;5;255m");
                    cout << textbox + "\n";
                    printf("\x1B[38;5;9m");
					cout << prefix + " ";
					textbox = "";
                    break;


                default:
                    textbox += key;
                    printf("\033[1D");
                    cout << key;
                    cursorFlash(3);
					
                }
            
        }
        cursorFlash(1);
    }
    
    return 0;
}