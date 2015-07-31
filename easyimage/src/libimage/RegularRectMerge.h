#ifndef _EASYIMAGE_REGULAR_RECT_MERGE_H_
#define _EASYIMAGE_REGULAR_RECT_MERGE_H_

#include <drag2d.h>

#include "Rect.h"

namespace eimage
{

class PixelCoveredLUT;
class RectPostProcessor;

class RegularRectMerge
{
public:
	RegularRectMerge(int width, int height, const std::vector<Rect>& rects,
		RectPostProcessor& proc);
	~RegularRectMerge();

	void Merge();

private:
	int ComputeCost(const Rect& r) const;

private:
	class RectCmp
	{
	public:
		bool operator () (const Rect& lhs, const Rect& rhs) const {
			return lhs.h * lhs.w < rhs.h * rhs.w;
		}
	}; // RectCmp

private:
	PixelCoveredLUT* m_covered_area;

	RectPostProcessor& m_proc;

	std::vector<Rect> m_rects;

}; // RegularRectMerge

}

#endif // _EASYIMAGE_REGULAR_RECT_MERGE_H_