#ifndef _EASYIMAGE_PIXEL_UNCOVERED_LUT_H_
#define _EASYIMAGE_PIXEL_UNCOVERED_LUT_H_

#include "Rect.h"

#include <cu/cu_stl.h>

namespace eimage
{

class PixelUncoveredLUT
{
public:
	PixelUncoveredLUT(int width, int height);
	~PixelUncoveredLUT();

	int GetUncoveredArea(int x, int y, int w, int h) const;
	int GetMultiArea(int x, int y, int w, int h) const;
	bool IntersectConstArea(int x, int y, int w, int h) const;

	void LoadRects(const CU_VEC<Rect>& rects);

private:
	int GetArea(int* area, int x, int y, int w, int h) const;

private:
	int* m_covered_count;
	int* m_empty_area;
	int* m_multi_area;

	int* m_const_count;
	int* m_const_area;

	int m_width, m_height;

}; // PixelUncoveredLUT

}

#endif // _EASYIMAGE_PIXEL_UNCOVERED_LUT_H_