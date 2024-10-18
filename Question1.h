#pragma once
#include "Question.h"
#include <random>

class Question1 : public Question
{
public:
	Question1();
	virtual ~Question1();
	virtual void run() override;

private:
	int seed;
	int gen_count;

	void changeGrid(Grid& grid);
	void askForParameters(int& size_x, int& size_y);
};