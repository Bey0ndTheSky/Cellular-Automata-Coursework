#include "SeedGenerator.h"
#include <vector>
#include <random>
#include <string>

void RandomSeedGenerator::generateSeeds() {
    seeds.clear();
    for (int i = 0; i < numSeeds; ++i) {
        seeds.push_back(distribution(generator));
    }
}

unsigned int RandomSeedGenerator::getSeed() {
    std::lock_guard<std::mutex> lock(rand_mutex_);
    if (seeds.empty()) {
        generateSeeds();
    }
    unsigned int seed = seeds.front();
    seeds.erase(seeds.begin());
    return seed;
}
