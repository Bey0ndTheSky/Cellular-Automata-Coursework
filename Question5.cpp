#include "Question5.h"
#include "SeedGenerator.h"
#include "Grid.h"
#include "PatternRecogniser.h"
#include "FileManager.h"
#include <iostream>
#include <memory>

Question5::Question5(std::initializer_list<std::string> patterns, std::initializer_list<std::string> patternsEX) {
	for (const auto& p : patterns) {
		this->patterns.push_back(FileManager::getInstance().loadPatternFromFile(p));
	}
	for (const auto& p : patternsEX) {
		this->patternsEX.push_back(FileManager::getInstance().loadPatternFromFile(p));
	}
}
Question5::~Question5() {}

void Question5::run() {

	printEstimatedERNs();
	for (auto& pr : patterns) {
		int ern = pr.calculateERN(height, width, alive);
		runSimulation(8, pr);
	}
	for (auto& pr : patternsEX) {
		int ern = pr.calculateERN(height, width, alive); 
		calculateExperimentalERN(pr);
		runSimulation(8, pr);
	}
}

void Question5::runSingleSimulation(PatternRecogniser pr) {
	int seed = RandomSeedGenerator::getInstance().getSeed();
	Grid grid(height, width, alive, seed);
	int gen_count = 0;
	while (!patternFound.load()) {
	for (int i = 0, frames = pr.getNumberOfFrames(); i < frames; i++)
	{
		if (pr.findPatternEX(grid)) {
			std::lock_guard<std::mutex> lock(ern_mutex);
			if (patternFound.load()) goto FOUND;
			std::cout << grid << std::endl;
	
			patternFound.store(true);
			grid = Grid(height, width, alive, seed, gen_count);
			FileManager::getInstance().saveSimToFile(pr.getName() + "/ERN/ERN " + std::to_string(height * width + alive), grid, seed, gen_count, alive);
			FileManager::getInstance().saveSimToFile(pr.getName() + "/ERN/CurrentERN", grid, seed, gen_count, alive);
			patternFound.store(true);
			Cycle(grid, pr.getNumberOfFrames(), 500);
			std::cout << pr.getName() << " with ERN of " << pr.calculateERN() << " in " << sim_counter.load() << " iterations" << std::endl;
			system("pause");
			goto FOUND;
			}
		grid.step();
		}
		sim_counter.fetch_add(1);
		gen_count = grid.getGeneratedCount();
		resetGrid(grid);
		FOUND:;
	}
}

void Question5::runSimulation(int number_of_threads, PatternRecogniser& pr) {
	std::vector<std::thread> threads;
	std::shared_ptr<FileLoader> loader;
	
	patternFound.store(false);
	sim_counter.store(0);
	std::cout << "Looking for " << pr.getName() << ": " << std::endl;

	for (int i = 0; i < number_of_threads; ++i) {
		threads.emplace_back(&Question5::runSingleSimulation, this, pr);
	}

	for (auto& thread : threads) {
		thread.join();
	}
}

void Question5::calculateExperimentalERN(PatternRecogniser& pr) {
	std::unique_ptr<FileLoader> ideal, actual;
	
	int ideal_height = height;
	try {
		ideal = FileManager::getInstance().loadSimFromFile(pr.getName() + "/ERN/IdealERN", false);
		actual = FileManager::getInstance().loadSimFromFile(pr.getName() + "/ERN/CurrentERN", true);
	}
	catch (const std::exception& e) {}
	if (ideal) {
		Grid ideal_grid = ideal->getGrid();
		Cycle(ideal_grid, pr.getNumberOfFrames(), 500);
	}

	actual ? (height = actual->getGrid().getGridX(), width = actual->getGrid().getGridY(), alive = actual->getGrid().getAliveCount())
		: (height += 15, width += 15, alive += 75);
	
	for (int i = 0, limit = height - ideal_height; i < limit; i++) {
		runSimulation(8, pr);
		height -= 1, width -= 1, alive -= 5;
	}
}

void Question5::printEstimatedERNs() {
	std::cout << "ESTIMATED THEORETICAL ERNs:\n\n";
	for (auto& pr : patterns) {
		int ern = pr.calculateERN();
		std::cout << pr.getName() << " ERN = " << ern << std::endl;
	}
	system("pause");
	system("cls");
}
