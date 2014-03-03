/** @file PackTest.cpp
	@author Jukka Jylänki

	@brief Implements a small sample application that takes an input list from a file and packs the rectangles using
		online variant of the GUILLOTINE data structure.

	This work is released to Public Domain, do whatever you want with it.
*/
#include <fstream>
#include <iostream>
#include <vector>

#include "Rect.h"
#include "GuillotineBinPack.h"

// To use different packers, uncomment one of the following.
// #include "MaxRectsBinPack.h"
// #include "ShelfBinPack.h"
// #include "SkylineBinPack.h"

using namespace std;

/// Reads the given file and returns a list of (width,height) pairs in the file to be used as an input sequence.
/// The file must contain a sequence of integers, two per line (each line specifies a single rectangle to pack).
/// See the input.txt for an example input format.
vector<RectSize> LoadRectanglesFromFile(const char *filename)
{
	vector<RectSize> rects;
	ifstream file(filename);
	while(file.good() && !file.eof())
	{
		RectSize r = { 0, 0 };
		file >> r.width;
		file >> r.height;

		if (r.width > 0 && r.height > 0)
			rects.push_back(r);
		else
			break; // Received bad input, stop here and use the items parsed so far.
	}

	cout << "Loaded " << rects.size() << " rectangles from file." << endl;

	return rects;
}

void main(int argc, char **argv)
{
	// The input rectangle list filename is taken from a command line parameter.
	// Call something like 'PackTest.exe input.txt'.
	if (argc < 2)
	{
		cout << "Usage: " << argv[0] << " <inputfile>" << endl;
		return;
	}
	// 'input' will contain the list of input rectangles to pack.
	vector<RectSize> input = LoadRectanglesFromFile(argv[1]);

	// The following lines show an example of how to pack the above list of input rectangles
	// as an online process using the GUILLOTINE data structure. If we want to do offline packing 
	// instead, sort the above input array first using std::sort with a custom predicate, and call 
	// void GuillotineBinPack::Insert(vector<RectSize> &rects, vector<Rect> &dst, bool merge, 
	//                                FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod);

	const int width = 500;
	const int height = 100;
	GuillotineBinPack bin(width, height); // Stores the current packing progress.

	// To use a different packer, replace the above line with one of the following, and adjust the bin.Insert
	// command below accordingly.
	// MaxRectsBinPack bin(width, height);
	// ShelfBinPack bin(width, height, false/true);
	// SkylineBinPack bin(width, height, false/true);

	int numPacked = 0; // The number of rectangles we packed successfully, for printing statistics at the end.
	for(size_t i = 0; i < input.size(); ++i)
	{
		// Pack the next rectangle in the input list.
		Rect output = bin.Insert(input[i].width, input[i].height,
			true, // Use the rectangle merge heuristic rule
			GuillotineBinPack::RectBestShortSideFit,
			GuillotineBinPack::SplitMinimizeArea);

		if (output.height == 0) // If the packer returns a degenerate height rect, it means the packing failed.
		{
			cout << "Failed to pack (" << input[i].width << ", " << input[i].height << ") into the bin. Skipping this one." << endl;
		}
		else // succeeded.
		{
			cout << "Packed the following rectangle: x: " << output.x << ", y: " << output.y
				<< ", width: " << output.width << ", height: " << output.height << "." << endl;
			++numPacked;
		}
	}

	// Finished the packing process.

	cout << "Successfully packed " << numPacked << " rectangles into a bin of size " << width << "x" << height << "." << endl;
	cout << "The bin is " << (bin.Occupancy() * 100.f) << "% full." << endl;
}
