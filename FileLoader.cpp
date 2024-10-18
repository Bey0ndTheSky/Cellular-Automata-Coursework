#include <iostream>
#include <string>
#include "FileLoader.h"
#include "FileManager.h"
#include "PatternRecogniser.h"

FileLoader::FileLoader(Grid grid, int seed, int gen_count, int alive, int steps, int ms) : grid(grid), ms(ms){
	this->seed = seed;
	this->gen_count = gen_count;
	this->alive = alive;
	this->steps = steps;
}
FileLoader::~FileLoader() {}

using namespace std;
void FileLoader::run(int steps) {
	int choice = -1;
	do {
		Cycle(grid, steps, ms);
		cout << "1. Run the current simulation more steps" << endl;
		cout << "2. Save current simulation to the disk" << endl;
		if (steps == 0){ cout << "3. Run until saved point" << endl; }
		cout << "0. Exit" << endl;
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "How many more steps do you want to run?" << endl;
			cin >> steps;
			break;

		case 2: {
			string filename = std::to_string(seed);
			FileManager::getInstance().saveSimToFile(filename, grid, seed, gen_count, alive);
			cout << "Saved simulation to " << filename << endl;
			break;
		}
		case 3: {
			if (steps == 0) { steps = this->steps; choice = 1; }
			break;
		}
			  
		default:
			choice = 0;
		}
	} while (choice == 1);
}

void FileLoader::runUntilPattern(std::initializer_list<std::string> patterns) {
	std::vector<PatternRecogniser> pattern_recognisers;
	for (const auto& p : patterns) {
		pattern_recognisers.push_back(FileManager::getInstance().loadPatternFromFile(p));
	}
	bool found = false;
	do {
		for (auto& r : pattern_recognisers) {
			found = found || r.findPattern(grid);
		}
		Cycle(grid,1, 300);
	} while (!found);
}