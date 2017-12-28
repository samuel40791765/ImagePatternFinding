#pragma once
#include <vector>

using std::vector;

typedef struct {
	int startpos;
	int width;
	int height;
	int times;
} Match;

typedef struct {
	int startpos;
	int findwdth;
	int findhght;
} PossibleMatches;

extern "C" void externalfunction(
	int img_width, 
	int img_height, 
	unsigned char *rgb,
	vector<PossibleMatches>& input,
	vector<Match>& output
	/*int *outputwidth, 
	int *outputheight, 
	int *outputtimes*/
);

extern "C" void checkpatterns(
	int img_width,
	int img_height,
	unsigned char *img_data,
	vector<unsigned char>& frequency,
	vector<PossibleMatches>& output
);