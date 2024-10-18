#pragma once
#include "Question.h"
#include <atomic>
#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <initializer_list>

class PatternRecogniser; class Pattern; class FileLoader;

class Question2 : public Question
{
public:
	Question2(std::initializer_list<std::string> patterns);
	virtual ~Question2();
	virtual void run() override;

private:
	int size_x, size_y;
	std::mutex grid_mutex;
	std::shared_ptr<FileLoader> loader;
	void runSingleSimulation(std::vector<PatternRecogniser> patterns);
	void runSimulation(int number_of_threads);
	std::atomic<int> sim_counter;
	std::atomic<bool> patternFound;
	std::vector<PatternRecogniser> patterns;
};