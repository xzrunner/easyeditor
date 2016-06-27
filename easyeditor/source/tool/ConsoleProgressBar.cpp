#include "ConsoleProgressBar.h"

#include <iostream>
#include <iomanip>

namespace ee
{

void ConsoleProgressBar::Print(unsigned int x, unsigned int n, unsigned int w) 
{
	if ( (x != n) && (x % (n/100+1) != 0) ) return;

	float ratio  =  x/(float)n;
	int   c      =  (int)(ratio * w);

	std::cout << std::setw(3) << (int)(ratio*100) << "% [";
	for (int x=0; x<c; x++) std::cout << "=";
	for (int x=c; x<w; x++) std::cout << " ";
	std::cout << "]\r" << std::flush;
}

// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
void ConsoleProgressBar::Print2(int x, int n, int r, int w)
{
	// Only update r times.
	if ( x % (n/r +1) != 0 ) return;

	// Calculate the ratio of complete-to-incomplete.
	float ratio = x/(float)n;
	int   c     = (int)(ratio * w);

	// Show the percentage complete.
	printf("%3d%% [", (int)(ratio*100) );

	// Show the load bar.
	for (int x=0; x<c; x++)
		printf("=");

	for (int x=c; x<w; x++)
		printf(" ");

	// ANSI Control codes to go back to the
	// previous line and clear it.
	printf("]\n\033[F\033[J");
}

}