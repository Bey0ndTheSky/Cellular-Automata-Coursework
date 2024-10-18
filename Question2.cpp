#include "Question2.h"
#include "SeedGenerator.h"
#include "Grid.h"
#include "PatternRecogniser.h"
#include "FileManager.h"
#include "FileLoader.h"
#include <iostream>
#include <thread>
#include <random>


Question2::Question2(std::initializer_list<std::string> patterns) : sim_counter(0), patternFound(false), patterns(){
	askForParameters(size_x, size_y);
	for (const auto& p : patterns) {
		this->patterns.push_back(FileManager::getInstance().loadPatternFromFile(p));
	}
}

Question2::~Question2() {}
using namespace std;

void Question2::run() {
	runSimulation(8);

	std::cout << "Press any button to reload the simulation:" << std::endl;
	system("pause");
	loader->run();
}

void Question2::runSingleSimulation(std::vector<PatternRecogniser> patterns) {
	int gen_count = 0;
	int seed = RandomSeedGenerator::getInstance().getSeed();

	Grid grid(size_x, size_y, alive, seed, 0);

	while (!patternFound.load()) {
		for (auto& pattern : patterns)
		{
			if (pattern.findPattern(grid)) {
				std::lock_guard<std::mutex> lock(grid_mutex);
				if (patternFound.load()) goto END;
				std::cout << grid << std::flush;
				std::cout << "Found " << pattern.getName() << " in " << sim_counter.load() << " simulations" << std::endl;

				FileManager::getInstance().saveSimToFile(pattern.getName() + "/" + std::to_string(seed), grid, seed, gen_count, alive);
				patternFound.store(true);
				
				loader = std::make_shared<FileLoader>(Grid(size_x, size_y, alive, seed, gen_count), seed, gen_count, alive, grid.getStepCount(),
					[&grid](Grid) -> int { return static_cast <int> (750 * (1 - (std::min(grid.getStepCount(), 150) / 100.0) * (1 - (1.0 / 3.0))));; }(grid));
				goto END;
			}
		}
		grid.step();
		if (grid.getAliveCount() == 0 || grid.getStepCount() > 100 + grid.getAliveCount())
		{
			gen_count = grid.getGeneratedCount();
			sim_counter.fetch_add(1);
			resetGrid(grid);
		}
	END:;
	}
}

void Question2::runSimulation(int number_of_threads) {
	std::vector<std::thread> threads;
	std::shared_ptr<FileLoader> loader;

	for (int i = 0; i < number_of_threads; ++i) {
		threads.emplace_back(&Question2::runSingleSimulation, this, patterns);
	}

	for (auto& thread : threads) {
		thread.join();
	}
}
