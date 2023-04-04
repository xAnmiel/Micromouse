#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
using namespace std;
#define MAZE_WIDTH 12
#define MAZE_HEIGHT 7
enum Direction { NORTH, EAST, SOUTH, WEST };
struct Node {
    int x, y;
    int distance;
};
Direction currentDirection = NORTH;
vector<vector<char>> mazeGrid = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', '#', ' ', '#', '#', '#', '#', '#', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', '#', '#', '#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', 'E', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

bool isValid(int x, int y, vector<vector<char>>& grid) {
    return x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT && grid[x][y] != '#';
}

void moveForward() {
    int newX = 0, newY = 0;
    switch (currentDirection) {
        case NORTH:
            newX = -1;
            break;
        case EAST:
            newY = 1;
            break;
        case SOUTH:
            newX = 1;
            break;
        case WEST:
            newY = -1;
            break;
    }
}

void turnLeft() {
    currentDirection = static_cast<Direction>((currentDirection + 3) % 4);
}

void turnRight() {
    currentDirection = static_cast<Direction>((currentDirection + 1) % 4);
}

void updateCurrentDirection(Direction newDirection) {
    while (currentDirection != newDirection) {
        turnRight();
    }
}

void updateFloodFillArray(int x, int y, int value, vector<vector<char>>& grid, vector<vector<bool>>& visited, vector<vector<int>>& floodFill) {
    if (!isValid(x, y, grid) || visited[x][y]) return;

    visited[x][y] = true;
    floodFill[x][y] = value;

    updateFloodFillArray(x - 1, y, value + 1, grid, visited, floodFill);
    updateFloodFillArray(x + 1, y, value + 1, grid, visited, floodFill);
    updateFloodFillArray(x, y - 1, value + 1, grid, visited, floodFill);
    updateFloodFillArray(x, y + 1, value + 1, grid, visited, floodFill);
}

Node findShortestPath(vector<vector<char>>& grid, vector<vector<int>>& floodFill) {
    int startX = -1, startY = -1;
    for (int i = 0; i < MAZE_WIDTH; ++i) {
        for (int j = 0; j < MAZE_HEIGHT; ++j) {
            if (grid[i][j] == 'S') {
                startX = i;
                startY = j;
                break;
            }
        }
        if (startX != -1 && startY != -1) break;
    }
    vector<vector<bool>> visited(MAZE_WIDTH, vector<bool>(MAZE_HEIGHT, false));

    updateFloodFillArray(startX, startY, 0, grid, visited, floodFill);

    int endX = -1, endY = -1;
    int minDistance = INT_MAX;
    for (int i = 0; i < MAZE_WIDTH; ++i) {
        for (int j = 0; j < MAZE_HEIGHT; ++j) {
            if (grid[i][j] == 'E' && visited[i][j] && floodFill[i][j] < minDistance) {
                endX = i;
                endY = j;
                minDistance = floodFill[i][j];
            }
        }
    }

    return {endX, endY, minDistance};
}

vector<vector<bool>> visited(MAZE_WIDTH, vector<bool>(MAZE_HEIGHT, false));

void setup(){}

void loop() {
vector<vector<int>> floodFillArray(MAZE_WIDTH, vector<int>(MAZE_HEIGHT, -1));
Node destination = findShortestPath(mazeGrid, floodFillArray);
cout << "La ruta más corta tiene una distancia de: " << destination.distance << endl;
delay(2000);
}