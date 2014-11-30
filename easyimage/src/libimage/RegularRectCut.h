#ifndef _EASYIMAGE_REGULAR_RECT_CUT_H_
#define _EASYIMAGE_REGULAR_RECT_CUT_H_

#include <drag2d.h>

#include "PixelEdgeTable.h"

namespace eimage
{

class RegularRectCut
{
public:
	struct Rect
	{
		Rect(int x, int y, int w, int h) 
			: x(x), y(y), w(w), h(h) {}

		int x, y;
		int w, h;
	};

public:
	RegularRectCut(const d2d::Image& image);
	~RegularRectCut();

	void AutoCut();

	const std::vector<Rect>& GetResult() { return m_result; }

	// statics
	int GetLeftArea() const { return m_left_area; }
	int GetUseArea() const;

private:
	void LoadPixels(const d2d::Image& image);

	void AutoCut(float limit);
	int AutoCut(int w, int h, int& ret_x, int& ret_y);

private:
	bool* m_pixels;
	int m_width, m_height;
	int m_left_area;

	float m_density;

	PixelEdgeTable* m_hor_table;

	std::vector<Rect> m_result;

}; // RegularRectCut

}

#endif // _EASYIMAGE_REGULAR_RECT_CUT_H_