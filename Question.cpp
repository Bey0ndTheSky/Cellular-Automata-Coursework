#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "Question.h"
#include "Grid.h"

Question::Question() {
    alive = 0;
    steps = 0;
}
Question::~Question()
{
}

using namespace std;

void Question::Cycle(Grid& grid, int steps, int ms) {
    for (int i = 0; i < steps; i++) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << grid << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        grid.step();
    }
    std::cout << "\033[2J\033[1;1H";
    std::cout << grid << std::endl;
}

void Question::changeGrid(Grid& grid) {
    int size_x, size_y;
    askForParameters(size_x, size_y);
    grid = Grid(size_x, size_y);
}

void Question::resetGrid(Grid& grid) {
    grid.reset();
	grid.generateRandGrid(alive);
}

void Question::askForParameters(int& size_x, int& size_y) {
    cout << "Enter grid size (width height): ";
    cin >> size_x >> size_y;

    cout << "Enter the initial number of alive cells: ";
    cin >> alive;
}