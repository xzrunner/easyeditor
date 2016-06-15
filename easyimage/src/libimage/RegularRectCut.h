#ifndef _EASYIMAGE_REGULAR_RECT_CUT_H_
#define _EASYIMAGE_REGULAR_RECT_CUT_H_

#include "Rect.h"

namespace ee { class Image; }

#include <vector>

#include <stdint.h>

namespace eimage
{

class PixelAreaLUT;

class RegularRectCut
{
public:
	RegularRectCut(const ee::Image& image);
	RegularRectCut(const ee::Image& image, const std::vector<Rect>& pre_rects);
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