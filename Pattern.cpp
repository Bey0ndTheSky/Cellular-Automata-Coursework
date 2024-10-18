#include <sstream>
#include "Pattern.h"

Pattern::Pattern(int size_x, int size_y, int offset_x, int offset_y) : grid(size_x, size_y, false),
offset_x(offset_x), offset_y(offset_y) {}

Pattern::Pattern() {}

Pattern::~Pattern() {}

std::istream& operator>>(std::istream& instream, Pattern& pattern)
{
    char ch;
    for (int i = 0; i < pattern.grid.getRows(); i++) { 
        instream.ignore(3);
        for (int j = 0; j < pattern.grid.getCols(); j++) {
            instream >> ch;
            if (ch == 'O') {
                pattern.grid(i, j) = true;
                instream.ignore(1);
            }
        }
        instream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return instream;
}

std::ostream& operator<<(std::ostream& outstream, const Pattern& pattern)
{
    for (int i = 0; i < pattern.grid.getRows(); i++) {
        outstream << "\t";
        for (int j = 0; j < pattern.grid.getCols(); j++) {
            outstream << "." << (pattern.grid(i, j) ? 'O' : ' ');
        }
        outstream << "." << "\n";
    }
    return outstream;
}

void Pattern::rotate90() {
    int temp = offset_x;
    offset_x = offset_y;
    offset_y = -temp;
    Array2D<uint8_t> new_grid(grid.getCols(), grid.getRows(),false);
    for (int i = 0; i < grid.getRows(); i++) {
        for (int j = 0; j < grid.getCols(); j++) {
            new_grid(j, grid.getRows() - i - 1) = grid(i, j);
        }
    }
    grid = std::move(new_grid);
}

void Pattern::mirror() {
    offset_x = -offset_x;
    grid.reverse();
}