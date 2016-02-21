#ifndef _EASYTEXPACKER_RECT_H_
#define _EASYTEXPACKER_RECT_H_

namespace etexpacker
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

	int tex_id;

	Rect()
		: tex_id(0) {}
};

}

#endif // _EASYTEXPACKER_RECT_H_