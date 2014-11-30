#ifndef _EASYIMAGE_RECT_H_
#define _EASYIMAGE_RECT_H_

namespace eimage
{

struct Rect
{
	Rect(int x, int y, int w, int h) 
		: x(x), y(y), w(w), h(h) {}

	int x, y;
	int w, h;
};

}

#endif // _EASYIMAGE_RECT_H_