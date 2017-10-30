#ifndef _EASYIMAGE_REGULAR_RECT_MERGE_H_
#define _EASYIMAGE_REGULAR_RECT_MERGE_H_

#include "Rect.h"

#include <cu/cu_stl.h>

namespace eimage
{

class PixelUncoveredLUT;
class RegularRectCondense;

class RegularRectMerge
{
public:
	RegularRectMerge(const CU_VEC<Rect>& rects, int width, 
		int height, bool* ori_pixels);
	~RegularRectMerge();

	void Merge();

	void GetResult(CU_VEC<Rect>& result) const;

private:
	int ComputeCost(const Rect& r, const CU_VEC<Rect>& rects) const;

private:
	int m_width, m_height;

	PixelUncoveredLUT* m_uncovered_lut;

	RegularRectCondense* m_condense;

}; // RegularRectMerge

}

#endif // _EASYIMAGE_REGULAR_RECT_MERGE_H_