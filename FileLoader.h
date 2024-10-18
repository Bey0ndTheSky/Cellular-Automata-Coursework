#pragma once
#include "Question.h"
#include "Grid.h"

class FileLoader : public Question
{
public:
	FileLoader(Grid grid, int seed, int gen_count, int alive, int steps = 0, int ms = 175);
	virtual ~FileLoader();
	virtual void run() override { run(steps); }
	void run(int steps);
	void runUntilPattern(std::initializer_list<std::string> patterns);
	Grid getGrid() const{ return grid; }

private:
	Grid grid;
	int seed;
	int ms;
	int gen_count;
};