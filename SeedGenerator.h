#include <vector>
#include <random>
#include <chrono>
#include <mutex>

/** Singleton for thread safety*/
class RandomSeedGenerator {
public:
    static RandomSeedGenerator& getInstance() {
        static RandomSeedGenerator instance;
        return instance;
    }
    
    RandomSeedGenerator(const RandomSeedGenerator&) = delete;
    RandomSeedGenerator& operator=(const RandomSeedGenerator&) = delete;

    unsigned int getSeed();

private:
    RandomSeedGenerator(int numSeeds = 100, int seed = std::chrono::system_clock::now().time_since_epoch().count())
        : numSeeds(numSeeds), generator(seed) {}
    int numSeeds;
    void generateSeeds();
    
    std::vector<unsigned int> seeds;
    std::mt19937 generator; 
    std::uniform_int_distribution<unsigned int> distribution{ 0, 4294967295 };
    std::mutex rand_mutex_;
};


