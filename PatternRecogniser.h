#pragma once
#include "Pattern.h"
#include <string>
class Grid;

class PatternRecogniser
{
public:
	PatternRecogniser(std::string pattern_name, std::vector<Pattern> patterns, int number_of_frames, int number_of_patterns, bool mirror);
	~PatternRecogniser();

	bool findPattern(const Grid& grid);

	/** Find a pattern checking for all possible frames at once, useful for finding ERN*/
	bool findPatternEX(const Grid& grid);

	std::string getName() const {
		return pattern_name;
	}
	void reset() { frame_count = 0; }
	int getNumberOfFrames() const { return number_of_frames; }

	int calculateERN();
	int calculateERN(int& height, int& width, int& alive);

private:
	std::string pattern_name;
	std::vector<Pattern> patterns;
	int number_of_patterns;
	int number_of_frames;
	int frame_count;
};