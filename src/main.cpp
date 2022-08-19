#ifdef __unix__

#include <stdlib.h>
#define WINDOWS 0
#define CLEAR "clear"

#elif defined(_WIN32) || defined(WIN32)

#include <windows.h>
#define WINDOWS 1
#define CLEAR "cls"

#endif

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <ctime>

using namespace std;

//pixel (2 unicode rectangle blocks)
const wstring PIXEL = L"\u2588\u2588\033[0m";

//color codes
const wstring GREEN = L"\033[32m";
const wstring RED = L"\033[31m";
const wstring BLUE = L"\033[34m";
const wstring PURPLE = L"\033[35m";
const wstring CYAN = L"\033[36m";
const wstring BLACK = L"\033[30m";
const wstring WHITE = L"\033[37m";
const wstring RESET = L"\033[0m";

const wstring lose_game = RED +
L"██    ██  ██████  ██    ██     ██       ██████  ███████ ███████\n"
L" ██  ██  ██    ██ ██    ██     ██      ██    ██ ██      ██\n"
L"  ████   ██    ██ ██    ██     ██      ██    ██ ███████ █████\n"   
L"   ██    ██    ██ ██    ██     ██      ██    ██      ██ ██\n"      
L"   ██     ██████   ██████      ███████  ██████  ███████ ███████\n" + RESET; 
                                                                
                                                                




class Screen {
    private:
        wstring screen[10][15];

    public:
        int score = 0;
        Screen() {
            //allow unicode output
            _setmode(_fileno(stdout), _O_U16TEXT);

            //initialize screen with empty spaces
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 15; j++) {
                    screen[i][j] = L"  ";
                }
            }
        }
        void printScrn() {
            system(CLEAR);
            wstring buffer = L"";
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 15; j++) {
                    buffer += screen[i][j];
                }
                buffer += L"\n";
            }
            buffer += L"score: " + to_wstring(score);
            wcout << buffer;
        }
        void setPixel(uint8_t x, uint8_t y) {
            screen[y][x] = GREEN+PIXEL;
        }
        void setPaddle(uint8_t x, uint8_t y) {
            screen[y][x] = PIXEL;
        }
        void removePixel(uint8_t x, uint8_t y) {
            screen[y][x] = L"  ";
        }
        wstring getPixel(uint8_t x, uint8_t y) {
            return screen[y][x];
        }
};

class Game {
    public:
        Screen scrn;
        void mainLoop() {
            //set once and pause for 1 second to allow the player time to think
            scrn.setPixel(x, y);
            scrn.setPaddle(paddleX, paddleY1);
            scrn.setPaddle(paddleX, paddleY2);
            scrn.setPaddle(paddleX, paddleY3);
            scrn.setPaddle(paddleX, paddleY4);
            scrn.printScrn();
            scrn.removePixel(x, y);
            scrn.removePixel(paddleX, paddleY1);
            scrn.removePixel(paddleX, paddleY2);
            scrn.removePixel(paddleX, paddleY3);
            scrn.removePixel(paddleX, paddleY4);
            Sleep(1000);
            if (rand() % 2 == 0) {
                down = true;
            }
            if (rand() % 2 == 0) {
                right = false;
            }
            while (true) {
                scrn.setPixel(x, y);
                scrn.setPaddle(paddleX, paddleY1);
                scrn.setPaddle(paddleX, paddleY2);
                scrn.setPaddle(paddleX, paddleY3);
                scrn.setPaddle(paddleX, paddleY4);
                scrn.printScrn();
                scrn.removePixel(x, y);
                scrn.removePixel(paddleX, paddleY1);
                scrn.removePixel(paddleX, paddleY2);
                scrn.removePixel(paddleX, paddleY3);
                scrn.removePixel(paddleX, paddleY4);
                if (down && right) {
                    y++;
                    x++;
                } else if (down && !right) {
                    y++;
                    x--;
                } else if (!down && right) {
                    y--;
                    x++;
                } else if (!down && !right) {
                    y--;
                    x--;
                }
                checkCollision();
                movePaddle();
                checkPaddleCollision();
                Sleep(70);
                if (GetAsyncKeyState(VK_ESCAPE)) {
                    break;
                }
            }
        }
        void checkCollision() {
            if (x == 0 && y == 0 || x == 0 && y == 9) { //corners
                right = !right;
                down = !down;
            }
            else if (x == 0) { //left wall
                right = !right;
            }
            else if (x == 14) {
                system(CLEAR);
                wcout << lose_game;

                Sleep(2000);
                //show cursor
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_CURSOR_INFO cursorInfo;
                GetConsoleCursorInfo(hConsole, &cursorInfo);
                cursorInfo.bVisible = TRUE;
                SetConsoleCursorInfo(hConsole, &cursorInfo);

                //quit
                exit(0);
            }
            else if (y == 0 || y == 9) { //top and bottom walls
                down = !down;
            }
        }
        void movePaddle() {
            if (GetAsyncKeyState(VK_UP)) {
                if (paddleY1 > 0) {
                    paddleY1--;
                    paddleY2--;
                    paddleY3--;
                    paddleY4--;
                }
            }
            else if (GetAsyncKeyState(VK_DOWN)) {

                if (paddleY1 < 9) {
                    paddleY1++;
                    paddleY2++;
                    paddleY3++;
                    paddleY4++;
                }
            }

        }
        void checkPaddleCollision() {
            if (x == paddleX && y == paddleY1 || x == paddleX && y == paddleY2 || x == paddleX && y == paddleY3) {
                right = !right;
                scrn.score++;
            }
        }
    private:
        uint8_t paddleX = 12;
        uint8_t paddleY1 = 5;
        uint8_t paddleY2 = 6;
        uint8_t paddleY3 = 7;
        uint8_t paddleY4 = 8;
        uint8_t x = 4;
        uint8_t y = rand() % 8 + 1;
        bool right = true;
        bool down = false;
};


int main() {
    srand((unsigned)time(NULL));
    cout << "Press enter to start" << endl;
    getchar();
    //hide the cursor
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO hCCI;
    hCCI.dwSize = 100;
    hCCI.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &hCCI);

    Game game;
    game.mainLoop();

    //show the cursor
    hCCI.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &hCCI);

    return 0;
}