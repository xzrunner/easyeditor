#ifndef _EASYIMAGE_RECT_H_
#define _EASYIMAGE_RECT_H_

namespace eimage
{

struct Rect
{
	Rect() {
		x = y = w = h = 0;
		is_const = false;
	}
	Rect(int x, int y, int w, int h) 
		: x(x), y(y), w(w), h(h), is_const(false) {}

	bool operator != (const Rect& r) const {
// 		return r.x != x || r.y != y 
// 			|| r.w != w || r.h != h;
		return !this->operator ==(r);
	}
	bool operator == (const Rect& r) const {
		return r.x == x && r.y == y 
			&& r.w == w && r.h == h;
	}

	int x, y;
	int w, h;

	bool is_const;
};

}

#endif // _EASYIMAGE_RECT_H_