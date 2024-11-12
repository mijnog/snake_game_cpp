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
void generateFood();
char getMapValue(int value);

// Map dimensions
const int mapWidth = 40;
const int mapHeight = 20;

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

// Score
int score = 0;

// Wall value
const int WALL = -3;

int main()
{
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed

    initscr(); // Start ncurses mode
    nodelay(stdscr, TRUE); // Non-blocking input
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
        usleep(300000); // Sleep for 100 milliseconds
    }
    clear();
    printw("Game Over! Your score: %d\n", score);
    refresh();
    usleep(2000000); // Sleep for 2 seconds before exiting
}

// Initialize the map
void initMap() {
    // Initialize position of snake head
    headxpos = mapWidth / 2;
    headypos = mapHeight / 2;
    direction = 0;
    // Fill the map
    for (int i = 0; i < mapSize; ++i) {
        map[i] = 0;
    }
    // Set the head position
    map[headypos * mapWidth + headxpos] = food;

    // Place the walls on the edges
    for (int x = 0; x < mapWidth; ++x) {
        map[x] = WALL; // Top edge
        map[(mapHeight - 1) * mapWidth + x] = WALL; // Bottom edge
    }
    for (int y = 0; y < mapHeight; ++y) {
        map[y * mapWidth] = WALL; // Left edge
        map[y * mapWidth + (mapWidth - 1)] = WALL; // Right edge
    }

    // Place the first piece of food
    generateFood();
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
        case WALL: return '#'; // Wall
    }
    return ' ';
}

// Change the direction of the snake
void changeDirection(int key) {
    switch (key) {
        case 'w': if (direction != 2) direction = 0; break;
        case 'd': if (direction != 3) direction = 1; break;
        case 's': if (direction != 0) direction = 2; break;
        case 'a': if (direction != 1) direction = 3; break;
    }
}

// Move the snake in the given direction
bool isInForgivenessState = false;  // Whether the snake is in a forgiveness state
int forgivenessCount = 0;           // How many game loops the snake has been in the forgiveness state

void moveSnake(int dx, int dy) {
    int newx = headxpos + dx;
    int newy = headypos + dy;

    // If snake is in forgiveness state, don't move and wait for the next loop
    if (isInForgivenessState) {
        forgivenessCount--;
        if (forgivenessCount <= 0) {
            isInForgivenessState = false;  // Reset forgiveness state after one loop
        }
        return;  // Return early to skip moving the snake
    }

    // Check if the snake hits the wall
    if (newx < 0 || newx >= mapWidth || newy < 0 || newy >= mapHeight || map[newy * mapWidth + newx] == WALL) {
        isInForgivenessState = true;  // Activate forgiveness state
        forgivenessCount = 1;  // Snake will stay still for 1 loop
        return;  // Return early, not allowing movement
    }

    // Check if the snake hits itself
    if (map[newy * mapWidth + newx] > 0) {
        isInForgivenessState = true;  // Activate forgiveness state
        forgivenessCount = 1;  // Snake will stay still for 1 loop
        return;  // Return early, not allowing movement
    }

    // Check if the snake eats the food
    if (map[newy * mapWidth + newx] == -2) {
        food++;
        score += 10; // Increase score by 10
        generateFood();
    } else {
        // Move the rest of the snake body
        for (int i = 0; i < mapSize; ++i) {
            if (map[i] > 0) map[i]--;
        }
    }

    // Move the snake head
    headxpos = newx;
    headypos = newy;

    // Set new head position
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
