#ifndef _EASYIMAGE_PIXEL_COVERED_LUT_H_
#define _EASYIMAGE_PIXEL_COVERED_LUT_H_

#include "Rect.h"

#include <cu/cu_stl.h>

namespace eimage
{

class PixelCoveredLUT
{
public:
	PixelCoveredLUT(int width, int height, const CU_VEC<Rect>& rects);
	~PixelCoveredLUT();

	int GetCoveredArea(int x, int y, int w, int h) const;

	void Remove(const Rect& r);
	void Insert(const Rect& r);

private:
	void LoadRects(const CU_VEC<Rect>& rects);

private:
	int* m_covered_count;
	int* m_area;

	int m_width, m_height;

}; // PixelCoveredLUT

}

#endif // _EASYIMAGE_PIXEL_COVERED_LUT_H_