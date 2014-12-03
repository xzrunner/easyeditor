#ifndef _LIBPACKER_RECT_H_
#define _LIBPACKER_RECT_H_

namespace libpacker
{
	
struct RectSize
{
	int width;
	int height;

	RectSize() 
		: width(-1), height(-1) {}
	RectSize(int w, int h) 
		: width(w), height(h) {}
};

struct Rect
{
	int x;
	int y;
	int width;
	int height;
};

}

#endif // _LIBPACKER_RECT_H_