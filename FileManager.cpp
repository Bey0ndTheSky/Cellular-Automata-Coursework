#include <iostream>
#include <mutex>
#include <string>
#include <fstream>
#include "FileManager.h"
#include "Grid.h"

using namespace std;

FileManager::FileManager() {
}

bool FileManager::saveSimToFile(const std::string& filename, const Grid& grid, int seed, int gen_count, int alive) {
    std::lock_guard<std::mutex> lock(file_mutex_);
    std::ofstream outFile(sim_path + filename + ".txt");
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false;
    }
    
    outFile << grid.getGridX() << " " << grid.getGridY() << endl;
    outFile << seed << " " << gen_count << endl;
    outFile << alive << " " << grid.getStepCount() << endl;
    outFile << grid;
    outFile.close();
    return true;
}

std::unique_ptr<FileLoader> FileManager::loadSimFromFile(const string& filename, bool reset) {
    int size_x, size_y, seed, alive, step_count, gen_count;
    ifstream inFile(sim_path + filename + ".txt");
    if (!inFile) {
        std::cerr << "Error opening file for reading. File may not exist: " << filename << std::endl;
        return nullptr;
    }
    inFile >> size_x >> size_y >> seed >> gen_count >> alive >> step_count;
    std::unique_ptr<FileLoader> loader;
    
    if (!reset){
        Grid gr(size_x, size_y);
        inFile >> gr;
        loader = std::make_unique<FileLoader>(gr, seed, gen_count, alive);
        cout << "Loading simulation with seed " << seed << " paused after " << step_count << " steps" << endl;
    }
    else {
        Grid gr(size_x, size_y, alive, seed, gen_count);
        loader = std::make_unique<FileLoader>(gr, seed, gen_count, alive, step_count);
        cout << "Loading simulation with seed " << seed << " at initial conditions" << endl;
    }

    inFile.close();
    return loader;
}

PatternRecogniser FileManager::loadPatternFromFile(const string& filename) {
    int size_x, size_y, patterns_no, mirror, frame_no, offset_x, offset_y;
    ifstream inFile(patt_path + filename + ".txt");
    if (!inFile) {
        std::cerr << "Error opening file for reading. File may not exist: " << filename << std::endl;
        throw std::runtime_error("File not found");
    }
    inFile >> size_x >> size_y >> patterns_no >> mirror >> frame_no;
    std::vector<Pattern> patterns(frame_no * patterns_no * (mirror + 1));
    for (int i = 0; i < frame_no; i++)
    {
        inFile >> offset_x >> offset_y;
        Pattern pattern(size_x, size_y, offset_x, offset_y);
        inFile >> pattern;
        patterns[i * patterns_no * (mirror + 1)] = pattern;
    }
    PatternRecogniser pr(filename, patterns, frame_no, patterns_no, mirror);

    inFile.close();
    return pr;

}