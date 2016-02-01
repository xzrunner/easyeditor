#ifndef _EASYIMAGE_REGULAR_RECT_MERGE_H_
#define _EASYIMAGE_REGULAR_RECT_MERGE_H_

#include "Rect.h"

#include <vector>

namespace eimage
{

class PixelUncoveredLUT;
class RegularRectCondense;

class RegularRectMerge
{
public:
	RegularRectMerge(const std::vector<Rect>& rects, int width, 
		int height, bool* ori_pixels);
	~RegularRectMerge();

	void Merge();

	void GetResult(std::vector<Rect>& result) const;

private:
	int ComputeCost(const Rect& r, const std::vector<Rect>& rects) const;

private:
	int m_width, m_height;

	PixelUncoveredLUT* m_uncovered_lut;

	RegularRectCondense* m_condense;

}; // RegularRectMerge

}

#endif // _EASYIMAGE_REGULAR_RECT_MERGE_H_