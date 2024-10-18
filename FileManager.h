#pragma once
#include "FileLoader.h"
#include "PatternRecogniser.h"
#include "Pattern.h"
#include <mutex>

// Singleton class
class FileManager {
public:
    static FileManager& getInstance() {
        static FileManager instance; 
        return instance; 
    }

    // Delete copy constructor and assignment operator
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    bool saveSimToFile(const std::string& filename, const Grid& grid, int seed, int gen_count, int alive);
    std::unique_ptr<FileLoader> loadSimFromFile(const std::string&, bool reset);

    PatternRecogniser loadPatternFromFile(const std::string& filename);

private:
    std::mutex file_mutex_;
    std::string sim_path = "Simulations/";
    std::string patt_path = "Patterns/";
    FileManager();
};