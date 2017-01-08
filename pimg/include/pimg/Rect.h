#ifndef _PIMG_RECT_H_
#define _PIMG_RECT_H_

#include <stdint.h>
#include <limits.h>

namespace pimg
{

class Rect
{
public:
	int16_t xmin, ymin;
	int16_t xmax, ymax;

	Rect() { MakeEmpty(); }
	Rect(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax)
		: xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}

	void MakeEmpty() {
		xmin = ymin = SHRT_MAX;
		xmax = ymax = SHRT_MIN;
	}
	bool IsValid() {
		return xmin != SHRT_MAX && ymin != SHRT_MAX && xmax != SHRT_MIN && ymax != SHRT_MIN;
	}

	int Width() const { return xmax - xmin; }
	int Height() const { return ymax - ymin; }

	float CenterX() const { return (xmin + xmax) * 0.5f; }
	float CenterY() const { return (ymin + ymax) * 0.5f; }

}; // Rect

}

#endif // _PIMG_RECT_H_