#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>
#include <thread>
#include <iostream>

using namespace std;
using namespace sf;

const int ROWS = 60;
const int COLS = 80;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 960;

const int CELL_SIZE = 16;


int REFRESH_INTERVAL_MS = 2000;
Color aliveCellColor = Color(70, 250, 70);
Color deadCellColor = Color(0, 80, 0);
Color outilneColor = Color::Black;


int random(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        srand(time(nullptr));
        initialized = true;
    }
    return min + rand() % (max - min + 1);
}

void initBoard(vector<vector<int>>& board) {
    
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (random(1,4) == 1) {
                board[i][j] = 1;
            }
        }
    }
}


void updateBoard(vector<vector<int>>& board) {
    
    vector<vector<int>> newBoard(board);

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int numNeighbors = 0;
            for (int k = i - 1; k <= i + 1; ++k) {
                for (int l = j - 1; l <= j + 1; ++l) {
                    if ((k != i || l != j) && k >= 0 && k < ROWS && l >= 0 && l < COLS && board[k][l] == 1) {
                        ++numNeighbors;
                    }
                }
            }

            if (board[i][j] == 1 && (numNeighbors < 2 || numNeighbors > 3)) {
                newBoard[i][j] = 0; 
            }
            else if (board[i][j] == 0 && numNeighbors == 3) {
                newBoard[i][j] = 1; 
            }
        }
    }

    board = newBoard;
}


void drawBoard(RenderWindow& window, const vector<vector<int>>& board) {
    RectangleShape cellShape(Vector2f(CELL_SIZE, CELL_SIZE));
    cellShape.setOutlineThickness(1.f);
    cellShape.setOutlineColor(Color::Black);

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cellShape.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            if (board[i][j] == 1) {
              
                cellShape.setFillColor(aliveCellColor);
            }
            else {
               
                cellShape.setFillColor(deadCellColor);
            }
            window.draw(cellShape);
        }
    }
}
int main() {
    
    cout << "Enter the refresh rate in milliseconds(2000 ms is 2s): ";
    cin >> REFRESH_INTERVAL_MS;
   
    vector<vector<int>> board(ROWS, vector<int>(COLS, 0));
    initBoard(board);

    
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Life (r,g,w,b to change color)");

    
    const Time refreshTime = milliseconds(REFRESH_INTERVAL_MS);
    Clock clock;

    
    while (window.isOpen()) {
        
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::KeyPressed) {
               
                switch (event.key.code) {
                case Keyboard::R:
                    aliveCellColor = Color::Red;
                    deadCellColor = Color(80, 0, 0);
                    
                    break;
                case Keyboard::G:
                    aliveCellColor = Color(70, 250, 70);
                    deadCellColor= Color(0, 80, 0);
                    
                    break;
                case Keyboard::B:
                    aliveCellColor = Color::Blue;
                    deadCellColor = Color(0, 0, 80);
                    
                    break;
                case Keyboard::W:
                    aliveCellColor = Color::White;
                    deadCellColor = Color::Black;
                    
                    break;
                }
            }
        }

        
        if (clock.getElapsedTime() > refreshTime) {
            updateBoard(board);
            clock.restart();
        }
       
        window.clear(Color::Black);

        drawBoard(window, board);

        window.display();
    }


    return 0;
}

