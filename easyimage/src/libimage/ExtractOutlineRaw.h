#ifndef _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
#define _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_

#include <ee/Vector.h>

namespace ee { class Image; }

#include <vector>

#include <stdint.h>

namespace eimage
{

class ExtractOutlineRaw
{
public:
	ExtractOutlineRaw(const ee::Image& image);

	void CreateBorderPoints();

	void CreateBorderLineAndMerge();

	void CreateBorderConvexHull();

	const std::vector<ee::Vector>& GetBorderLine() const {
		return m_border_line;
	}
	const std::vector<ee::Vector>& GetBorderPoints() const {
		return m_border_points;
	}
	const std::vector<ee::Vector>& GetBorderLineMerged() const {
		return m_border_line_merged;
	}
	const std::vector<ee::Vector>& GetConvexHull() const {
		return m_convex_hull;
	}

private:
	void CreateBorderLine();
	void MergeBorderLine();

	bool IsPixelBorder(int x, int y) const;

	bool IsPixelTransparente(int x, int y) const;
	bool IsPixelTransparente(bool* flag, int x, int y) const;

	bool IsNearby(const ee::Vector& p0, const ee::Vector& p1) const;

	static void MergeLine(const std::vector<ee::Vector>& src, std::vector<ee::Vector>& dst);

private:
	const uint8_t* m_pixels;
	int m_width, m_height;

	std::vector<ee::Vector> m_border_points;
	std::vector<ee::Vector> m_border_axis_line;

	std::vector<ee::Vector> m_border_line;
	std::vector<ee::Vector> m_border_line_merged;

	std::vector<ee::Vector> m_convex_hull;

}; // ExtractOutlineRaw

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
