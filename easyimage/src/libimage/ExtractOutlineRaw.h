#ifndef _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
#define _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_

#include <SM_Vector.h>

#include <vector>

#include <stdint.h>

namespace ee { class Image; }

namespace eimage
{

class ExtractOutlineRaw
{
public:
	ExtractOutlineRaw(const ee::Image& image);

	void CreateBorderPoints();

	void CreateBorderLineAndMerge();

	void CreateBorderConvexHull();

	const std::vector<sm::vec2>& GetBorderLine() const {
		return m_border_line;
	}
	const std::vector<sm::vec2>& GetBorderPoints() const {
		return m_border_points;
	}
	const std::vector<sm::vec2>& GetBorderLineMerged() const {
		return m_border_line_merged;
	}
	const std::vector<sm::vec2>& GetConvexHull() const {
		return m_convex_hull;
	}

private:
	void CreateBorderLine();
	void MergeBorderLine();

	bool IsPixelBorder(int x, int y) const;

	bool IsPixelTransparente(int x, int y) const;
	bool IsPixelTransparente(bool* flag, int x, int y) const;

	bool IsNearby(const sm::vec2& p0, const sm::vec2& p1) const;

	static void MergeLine(const std::vector<sm::vec2>& src, std::vector<sm::vec2>& dst);

private:
	const uint8_t* m_pixels;
	int m_width, m_height;

	std::vector<sm::vec2> m_border_points;
	std::vector<sm::vec2> m_border_axis_line;

	std::vector<sm::vec2> m_border_line;
	std::vector<sm::vec2> m_border_line_merged;

	std::vector<sm::vec2> m_convex_hull;

}; // ExtractOutlineRaw

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
