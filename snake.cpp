/* Snake Game using C++ 
developed by TheKittyKat, 
improved by Nazim Nazari 
December 2017 */

#include <iostream>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <ncurses.h>
#include <unistd.h> // For usleep()

using namespace std;

void run();
void printMap();
void initMap();
void moveSnake(int dx, int dy);
void update();
void changeDirection(int key);
void clearScreen();
void generateFood();

char getMapValue(int value);

// Map dimensions
const int mapWidth = 40;
const int mapHeight = 40;

const int mapSize = mapWidth * mapHeight;

// The tile values for the map
int map[mapSize];

// Snake head details
int headxpos;
int headypos;
int direction;

// Amount of food the snake has (How long the body is)
int food = 4;

// Determine if game is running
bool running;

int main()
{
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed

    initscr(); // Start ncurses mode
    nodelay(stdscr, TRUE); // Non-blocking input
    keypad(stdscr, TRUE); // Enable function keys
    noecho(); // Don't echo pressed keys to the screen
    curs_set(FALSE); // Hide the cursor

    run();

    endwin(); // End ncurses mode
    return 0;
}

// Main game function
void run()
{
    // Initialize the map
    initMap();
    running = true;
    while (running) {
        // If a key is pressed
        int ch = getch();
        if (ch != ERR) {
            changeDirection(ch);
        }
        update();
        printMap();
        usleep(100000); // Sleep for 100 milliseconds
    }
    printw("Game Over!\n");
    refresh();
    usleep(2000000); // Sleep for 2 seconds before exiting
}

// Initialize the map
void initMap() {
    // Initialize position of snake head
    headxpos = mapWidth / 2;
    headypos = mapHeight / 2;
    direction = 0;
    // Place the food
    generateFood();
    // Fill the map
    for (int i = 0; i < mapSize; ++i) {
        map[i] = 0;
    }
    // Set the head position
    map[headypos * mapWidth + headxpos] = -1;
}

// Print the map to the console
void printMap() {
    clear();
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            mvaddch(y, x, getMapValue(map[y * mapWidth + x]));
        }
    }
    refresh();
}

// Get the char representation of the map value
char getMapValue(int value) {
    if (value > 0) return 'o';
    switch (value) {
        case -1: return 'O'; // Snake head
        case -2: return 'X'; // Food
    }
    return ' ';
}

// Clear the screen
void clearScreen() {
    // This is now handled by ncurses with `clear()`
}

// Change the direction of the snake
void changeDirection(int key) {
    switch (key) {
        case KEY_UP: direction = 0; break;
        case KEY_RIGHT: direction = 1; break;
        case KEY_DOWN: direction = 2; break;
        case KEY_LEFT: direction = 3; break;
    }
}

// Move the snake in the given direction
void moveSnake(int dx, int dy) {
    int newx = headxpos + dx;
    int newy = headypos + dy;
    
    // Check if the snake hits the wall
    if (newx < 0 || newx >= mapWidth || newy < 0 || newy >= mapHeight) {
        running = false;
        return;
    }
    
    // Check if the snake hits itself
    if (map[newy * mapWidth + newx] != 0 && map[newy * mapWidth + newx] != -2) {
        running = false;
        return;
    }
    
    // Check if the snake eats the food
    if (map[newy * mapWidth + newx] == -2) {
    food++;
    generateFood();
} else {
    // Move the snake head
    headxpos = newx;
    headypos = newy;
    map[headypos * mapWidth + headxpos] = -1;

    // Update the rest of the snake body
    for (int i = 0; i < mapSize; ++i) {
        if (map[i] > 0) map[i]--;
    }
    map[headypos * mapWidth + headxpos] = food;

}
    // Move the snake head
    headxpos = newx;
    headypos = newy;
    map[headypos * mapWidth + headxpos] = -1;
    
    // Update the rest of the snake body
    for (int i = 0; i < mapSize; ++i) {
        if (map[i] > 0) map[i]--;
    }
    map[headypos * mapWidth + headxpos] = food;
}

// Update the game state
void update() {
    switch (direction) {
        case 0: moveSnake(0, -1); break;
        case 1: moveSnake(1, 0); break;
        case 2: moveSnake(0, 1); break;
        case 3: moveSnake(-1, 0); break;
    }
}

// Generate food in a random position
void generateFood() {
    int x, y;
    do {
        x = rand() % mapWidth;
        y = rand() % mapHeight;
    } while (map[y * mapWidth + x] != 0);
    map[y * mapWidth + x] = -2;
}