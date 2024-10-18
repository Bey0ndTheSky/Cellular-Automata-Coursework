#include <memory>
#include <thread>
#include <vector>
#include "Grid.h"
#include "PatternRecogniser.h"

PatternRecogniser::PatternRecogniser(std::string pattern_name, std::vector<Pattern> patterns, int number_of_frames, int number_of_patterns, bool mirror) :
	pattern_name(pattern_name), patterns(patterns), number_of_frames(number_of_frames), number_of_patterns(number_of_patterns * (mirror + 1)){
	frame_count = 0;

	for (int i = 0; i < number_of_frames; i++) {
		Pattern pattern = patterns[i * this->number_of_patterns];
		for (int j = 1; j < this->number_of_patterns; j++) {
			pattern.rotate90();
			if (j == number_of_patterns) { pattern.mirror(); }
			this->patterns[i * this->number_of_patterns + j] = pattern;
		}
	}
}

PatternRecogniser::~PatternRecogniser() {}

bool PatternRecogniser::findPatternEX(const Grid& grid) {
	for (int i = 0; i < number_of_frames * number_of_patterns; i++) {
		Pattern pattern = patterns[i];
		if (grid == pattern){
			frame_count++;
			return frame_count == number_of_frames;
		}
	}
	frame_count = 0;
	return false;
}

bool PatternRecogniser::findPattern(const Grid& grid) {
	for (int i = 0; i < number_of_patterns; i++) {
		Pattern pattern = patterns[i + (frame_count * number_of_patterns)];
		if (grid == pattern) {
			frame_count++;
			return frame_count == number_of_frames;
		}
	}
	frame_count = 0;
	return false;
}

int PatternRecogniser::calculateERN(int& height, int& width, int& alive) {

	auto net_movement = [](int offset, int& z, int& min_z, int& max_z) -> void {
		z += offset;
		min_z = (z < min_z) ? z : min_z;
		max_z = (z > max_z) ? z : max_z;
	};
	int x, min_x, max_x, y, min_y, max_y;
	x = y = min_x = min_y = max_x = max_y = 0;

	Pattern pattern = patterns[0];
	alive = 9999;
	height = pattern.getGrid().getRows() - 2;
	width = pattern.getGrid().getCols() - 2;
	
	for (int i = 0; i < number_of_frames; i++) {	
		pattern = patterns[i * number_of_patterns];
		Array2D<uint8_t> pattern_grid = pattern.getGrid();
		alive = std::min(static_cast<int>(std::count(pattern_grid.begin(), pattern_grid.end(), 1)), alive);

		net_movement(pattern.getOffsetX(), x, min_x, max_x);
		net_movement(pattern.getOffsetY(), y, min_y, max_y);
	}
	height = height + max_x - min_x;
	width = width + max_y - min_y;
	return ( width * height + alive);
}

int PatternRecogniser::calculateERN() {
	int height = 0, width = 0, alive = 0;
	return calculateERN(height, width, alive);
}

