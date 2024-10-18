#pragma once
#include "2D Array.h"
#include "Grid.h"

class Grid;
class Pattern
{
public:
	Pattern();
	Pattern(int size_x, int size_y, int offset_x, int offset_y);
	~Pattern();

	void rotate90();
	void mirror();

	int getOffsetX() const {
		return offset_x;
	}

	int getOffsetY() const {
		return offset_y;
	}

	Array2D<uint8_t> getGrid() const {
		return grid;
	}
	friend std::istream& operator>>(std::istream& instream, Pattern& pattern);
	friend std::ostream& operator<<(std::ostream& outstream, const Pattern& pattern);
	friend bool Grid::operator==(const Pattern& pattern) const;

private:
	Array2D<uint8_t> grid;
	int offset_x;
	int offset_y;

	
};