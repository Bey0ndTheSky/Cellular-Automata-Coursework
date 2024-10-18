#include "Grid.h"
#include "Pattern.h"
#include <sstream>

void Grid::generateRandGrid(int alive) {
    step_count = 0;
    for(int i = 0; i < alive; i++)
    {   
        std::uniform_int_distribution<int> x_dist(1, grid_x);
        std::uniform_int_distribution<int> y_dist(1, grid_y);
        int x = x_dist(generator);
        int y = y_dist(generator);
        generated_count += 2;

        while (grid(x,y) == true) {
            x = x_dist(generator);
            y = y_dist(generator);
            generated_count += 2;
        }
        grid(x, y) = true;
    }
}

Grid::Grid(int size_x, int size_y) : generator(0), grid(size_x + 2, size_y + 2, false){
    grid_x = size_x;
    grid_y = size_y;

    generated_count = 0;
    step_count = 0;
    alive_count = 0;
}

Grid::Grid(int size_x, int size_y, int alive, int seed, int gen_count) : Grid(size_x, size_y) {
    generator.seed(seed);
    generated_count = gen_count;
    generator.discard(gen_count);
    generateRandGrid(alive);
}

Grid::~Grid() {}

void Grid::step() {
    Array2D<uint8_t> copy_grid = grid;
    alive_count = 0;

    for (int i = 1; i <= grid_x; i++) {
        for (int j = 1; j <= grid_y; j++) {
            int sum = copy_grid(i - 1, j - 1) + copy_grid(i - 1, j) + copy_grid(i - 1, j + 1) 
                + copy_grid(i, j - 1) + copy_grid(i, j + 1) 
                + copy_grid(i + 1, j - 1) + copy_grid(i + 1, j) + copy_grid(i + 1, j + 1);

            switch (sum)
            {
                case 2:
                    alive_count += grid(i, j);
                    break; 
                case 3:
                    grid(i, j) = true;
                    alive_count++;
                    break;

                default:
                    grid(i, j) = false; 
                    break;
            }
        }
    }
    step_count++;
}

std::ostream& operator<<(std::ostream& outstream, const Grid& grid)
{
    for (int i = 1; i <= grid.getGridX(); i++) {
        outstream << "\t";
        for (int j = 1; j <= grid.getGridY(); j++) {
            outstream << "." << (grid.grid(i, j) ? 'O' : ' ');
        }
        outstream << "." << "\n";
    }
    return outstream;
}

std::istream& operator>>(std::istream& instream, Grid& grid)
{
    char ch;
    instream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int i = 1; i <= grid.getGridX(); i++) { 
        instream.ignore(2);
        for (int j = 1; j <= grid.getGridY(); j++) {
            instream >> ch;
            if (ch == 'O') {
                grid.grid(i, j) =  true;
                instream.ignore(1);
            }
        }
        instream.ignore(1);
    }
    return instream;
}

bool Grid::operator==(const Pattern& pattern) const {
    Array2D<uint8_t> pattern_grid = pattern.getGrid();
    auto pattern_start = std::find(pattern_grid.begin(), pattern_grid.end(), 1);
    std::pair<int, int> ix = pattern_grid.get2D(std::distance(pattern_grid.begin(), pattern_start));

    int max_i = getGridX() - pattern_grid.getRows() + ix.first + 2;
    int max_j = getGridY() - pattern_grid.getCols() + ix.second + 2;

    for (int i = ix.first; i <= max_i; i++) {
        for (int j = ix.second; j <= max_j; j++) {
            if (!grid(i, j)) continue;
            for (int x = 0; x < pattern_grid.getRows(); x++) {
                for (int y = 0; y < pattern_grid.getCols(); y++) {
                    if (pattern_grid(x, y) != grid(i + x - ix.first, j + y - ix.second)) goto outer_loop;
                }
            }
            return true;
        outer_loop:;
        }
    }
    return false;
}