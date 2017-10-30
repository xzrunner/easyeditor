#ifndef _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
#define _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <SM_Vector.h>

#include <stdint.h>

namespace eimage
{

class ExtractOutlineRaw
{
public:
	ExtractOutlineRaw(const uint8_t* pixels, int w, int h);

	void CreateBorderPoints();

	void CreateBorderLineAndMerge();

	void CreateBorderConvexHull();

	const CU_VEC<sm::vec2>& GetBorderLine() const {
		return m_border_line;
	}
	const CU_VEC<sm::vec2>& GetBorderPoints() const {
		return m_border_points;
	}
	const CU_VEC<sm::vec2>& GetBorderLineMerged() const {
		return m_border_line_merged;
	}
	const CU_VEC<sm::vec2>& GetConvexHull() const {
		return m_convex_hull;
	}

private:
	void CreateBorderLine();
	void MergeBorderLine();

	bool IsPixelBorder(int x, int y) const;

	bool IsPixelTransparente(int x, int y) const;
	bool IsPixelTransparente(bool* flag, int x, int y) const;

	bool IsNearby(const sm::vec2& p0, const sm::vec2& p1) const;

	static void MergeLine(const CU_VEC<sm::vec2>& src, CU_VEC<sm::vec2>& dst);

private:
	const uint8_t* m_pixels;
	int m_width, m_height;

	CU_VEC<sm::vec2> m_border_points;
	CU_VEC<sm::vec2> m_border_axis_line;

	CU_VEC<sm::vec2> m_border_line;
	CU_VEC<sm::vec2> m_border_line_merged;

	CU_VEC<sm::vec2> m_convex_hull;

}; // ExtractOutlineRaw

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
