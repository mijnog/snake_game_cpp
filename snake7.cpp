#include <iostream>
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
void setDifficulty(int difficulty);

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

// Difficulty and wall option
bool wallsEnabled = true; // Flag to remove walls
int difficulty = 5; // Default difficulty (speed)

// Main function
int main()
{
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed

    initscr(); // Start ncurses mode
    noecho(); // Don't echo pressed keys to the screen
    curs_set(FALSE); // Hide the cursor

    // Temporarily disable nodelay and allow for user input
    timeout(-1); // This ensures getch() waits for input

    // Ask for difficulty level and wall option
    clear();
    printw("Choose difficulty (1-9): ");
    refresh();
    int diff = getch() - '0'; // Convert char to int
    if (diff >= 1 && diff <= 9) difficulty = diff;

    printw("\nEnable walls? (y/n): ");
    refresh();
    char wallChoice = getch();
    wallsEnabled = (wallChoice == 'y' || wallChoice == 'Y');

    timeout(0); // Re-enable non-blocking input for the game loop
    run(); // Start the game

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
        usleep(1000000 / difficulty); // Adjust speed based on difficulty level
    }

    // Display score before GAME OVER message
    clear();
    printw("Score: %d\n", score);
    printw("GAME OVER!\n");
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

    // Place the walls on the edges if enabled
    if (wallsEnabled) {
        for (int x = 0; x < mapWidth; ++x) {
            map[x] = WALL; // Top edge
            map[(mapHeight - 1) * mapWidth + x] = WALL; // Bottom edge
        }
        for (int y = 0; y < mapHeight; ++y) {
            map[y * mapWidth] = WALL; // Left edge
            map[y * mapWidth + (mapWidth - 1)] = WALL; // Right edge
        }
    }

    // Place the first piece of food
    generateFood();
}

// Print the map to the console
void printMap() {
    clear();
    // Print the score at the top of the screen
    mvprintw(0, 0, "Score: %d", score);
    
    // Print the game map below the score
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            mvaddch(y + 1, x, getMapValue(map[y * mapWidth + x]));
        }
    }
    refresh();
}

// Get the char representation of the map value
char getMapValue(int value) {
    if (value > 0) return 'o'; // Snake body
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
void moveSnake(int dx, int dy) {
    int newx = headxpos + dx;
    int newy = headypos + dy;

    // If walls are not enabled, wrap the snake around the screen
    if (!wallsEnabled) {
        if (newx < 0) newx = mapWidth - 1;
        if (newx >= mapWidth) newx = 0;
        if (newy < 0) newy = mapHeight - 1;
        if (newy >= mapHeight) newy = 0;
    } 
    else { // If walls are enabled, check if the snake hits the wall
        if (newx < 0 || newx >= mapWidth || newy < 0 || newy >= mapHeight || map[newy * mapWidth + newx] == WALL) {
            running = false; // End the game if the snake hits a wall
            return;
        }
    }

    // Check if the snake hits itself (excluding the head's position)
    if (map[newy * mapWidth + newx] > 0) {
        running = false; // End the game if the snake hits its own body
        return;
    }

    // Check if the snake eats the food
    if (map[newy * mapWidth + newx] == -2) {
        food++;
        score += 10 * difficulty; // Increase score by 10 times the difficulty level
        generateFood(); // Generate new food
    } else {
        // Move the snake body
        for (int i = 0; i < mapSize; ++i) {
            if (map[i] > 0) {
                map[i]--; // Move the body forward
            }
        }
    }

    // Move the snake head
    headxpos = newx;
    headypos = newy;

    // Set new head position
    map[headypos * mapWidth + headxpos] = food; // Set head in new position
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
    } while (map[y * mapWidth + x] != 0); // Make sure the food doesn't spawn on top of the snake
    map[y * mapWidth + x] = -2; // Place food
}