#include "Question1.h"
#include "FileManager.h"
#include "SeedGenerator.h"
#include "Grid.h"
#include <iostream>
#include <string>


Question1::Question1() {
	gen_count = 0;
	seed = RandomSeedGenerator::getInstance().getSeed();
}

Question1::~Question1() {
}

using namespace std;
void Question1::run() {
	int size_x, size_y;
	askForParameters(size_x, size_y);
	Grid grid = Grid(size_x, size_y, alive, seed, 0);
	int choice = -1;
	int steps = this->steps;
	do{
		Cycle(grid, steps);

		cout << "1. Create a simulation with a different initial distribution" << endl;
		cout << "2. Run a simulation with different grid parameters" << endl;
		cout << "3. Run the current simulation more steps" << endl;
		cout << "4. Save current simulation to the disk" << endl;
		cout << "0. Exit" << endl;
		cin >> choice;
		
		switch (choice) {
		case 1:
			steps = this->steps;
			gen_count = grid.getGeneratedCount();
			resetGrid(grid);
			break;

		case 2:
			gen_count = grid.getGeneratedCount();
			changeGrid(grid);
			break;

		case 3:
			cout << "How many more steps do you want to run?" << endl;
			cin >> steps;
			break;
		
		case 4: {
			string filename = "manual/" + std::to_string(seed);
			FileManager::getInstance().saveSimToFile(filename + ".txt", grid, seed, gen_count, alive);
			cout << "Saved simulation to " << filename << endl;}
			break;

		default:
			choice = 0;
		}
	} while (choice > 0 && choice < 4);
}

void Question1::changeGrid(Grid& grid) {
	int size_x, size_y;
	askForParameters(size_x, size_y);
	grid = Grid(size_x, size_y, alive, seed, gen_count);
}

void Question1::askForParameters(int& size_x, int& size_y) {
	Question::askForParameters(size_x, size_y);

	cout << "Enter the number of simulation steps: ";
	cin >> steps;
}