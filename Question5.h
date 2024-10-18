#pragma once
#include "Question.h"
#include <atomic>
#include <vector>
#include <string>
#include <mutex>
#include <initializer_list>

class PatternRecogniser;

class Question5: public Question
{
public:
	Question5(std::initializer_list<std::string> patterns = { "block", "beehive", "blinker", "toad", "glider"},
		std::initializer_list<std::string> patternsEX = { "lwss" });
	virtual ~Question5();
	virtual void run() override;
	void printEstimatedERNs();
	void runSingleSimulation(PatternRecogniser pr);
	void runSimulation(int number_of_threads, PatternRecogniser& pr);
	void calculateExperimentalERN(PatternRecogniser& pr);

private:
	int width = 0;
	int height = 0;
	std::mutex ern_mutex;
	std::atomic<int> sim_counter;
	std::atomic<bool> patternFound;
	std::vector<PatternRecogniser> patterns;
	std::vector<PatternRecogniser> patternsEX;
};