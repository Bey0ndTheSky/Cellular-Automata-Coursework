#pragma once

class Grid;

class Question
{
public:
	Question();

	virtual ~Question();

	virtual void run() = 0;

	void Cycle(Grid& grid, int steps, int ms = 175);

	void askForParameters(int& size_x, int& size_y);

	void changeGrid(Grid& grid);

	void resetGrid(Grid& grid);

protected:
	int alive = 0;
	int steps;
};


