#pragma once
#include <random>
#include "2D Array.h"

class Pattern;

class Grid
{
    private:
        int grid_x;
        int grid_y;

        int step_count;
        int alive_count;
        int generated_count;

        Array2D<uint8_t> grid;
        std::default_random_engine generator;

    public:
        Grid(int size_x, int size_y, int alive, int seed, int gen_count = 0);
        Grid(int size_x, int size_y);
        ~Grid();

        void generateRandGrid(int alive);
        void step();
        void reset(){ grid = Array2D<uint8_t>(grid_x + 2, grid_y + 2, false); }
        friend std::ostream& operator<<(std::ostream& outstream, const Grid& grid);
        friend std::istream& operator>>(std::istream& instream, Grid& grid);
        bool operator==(const Pattern& pattern) const;

        int getGridX() const {
            return grid_x;
        }

        int getGridY() const {
            return grid_y;
        }

        int getStepCount() const {
            return step_count;
        }

        int getAliveCount() const {
            return alive_count;
        }

        int getGeneratedCount() const {
            return generated_count;
        }
    };