// from http://www.ross.click/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/

#ifndef _EASYEDITOR_CONSOLE_PROGRESS_BAR_H_
#define _EASYEDITOR_CONSOLE_PROGRESS_BAR_H_

namespace ee
{

class ConsoleProgressBar
{
public:
	static void Print(unsigned int x, unsigned int n, unsigned int w = 50);

	static void Print2(int x, int n, int r, int w);

}; // ConsoleProgressBar

}

#endif // _EASYEDITOR_CONSOLE_PROGRESS_BAR_H_