#ifndef _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
#define _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_

#include <drag2d.h>

namespace eimage
{

class ExtractOutlineRaw
{
public:
	ExtractOutlineRaw(const d2d::Image& image);

	void Trigger();

	const std::vector<d2d::Vector>& GetBorderLine() const {
		return m_border_line;
	}
	const std::vector<d2d::Vector>& GetBorderPoints() const {
		return m_border_points;
	}
	const std::vector<d2d::Vector>& GetBorderLineMerged() const {
		return m_border_merged;
	}

private:
	void CreateBorderLine();
	void CreateBorderPoints();
	void MergeBorderLine();

	bool IsPixelBorder(int x, int y) const;

	bool IsPixelTransparente(int x, int y) const;
	bool IsPixelTransparente(bool* flag, int x, int y) const;

	bool IsNearby(const d2d::Vector& p0, const d2d::Vector& p1) const;

	static void MergeLine(const std::vector<d2d::Vector>& src, std::vector<d2d::Vector>& dst);

private:
	const byte* m_pixels;
	int m_width, m_height;

	std::vector<d2d::Vector> m_border_points;
	std::vector<d2d::Vector> m_border_line;
	std::vector<d2d::Vector> m_border_merged;

}; // ExtractOutlineRaw

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_RAW_H_
