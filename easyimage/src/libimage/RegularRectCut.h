#ifndef _EASYIMAGE_REGULAR_RECT_CUT_H_
#define _EASYIMAGE_REGULAR_RECT_CUT_H_

#include <drag2d.h>

#include "Rect.h"

namespace eimage
{

class PixelAreaLUT;

class RegularRectCut
{
public:
	RegularRectCut(const d2d::Image& image);
	RegularRectCut(const uint8_t* pixels, int width, int height);
	~RegularRectCut();

	void AutoCut();
	
	const std::vector<Rect>& GetResult() { return m_result; }

	// statics
	int GetLeftArea() const { return m_left_area; }
	int GetUseArea() const;

private:
	void LoadPixels(const uint8_t* pixels, int width, int height);

	void AutoCutWithLimit(float limit);
	int CalBestRectPos(int w, int h, int& ret_x, int& ret_y);

	void PoseProcessResult();

private:
	bool* m_pixels;
	int m_width, m_height;
	int m_left_area;

	float m_density;

	PixelAreaLUT* m_area_array;	// real data

	std::vector<Rect> m_result;

}; // RegularRectCut

}

#endif // _EASYIMAGE_REGULAR_RECT_CUT_H_