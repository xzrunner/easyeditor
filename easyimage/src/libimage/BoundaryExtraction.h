#ifndef _EASYIMAGE_BOUNDARY_EXTRACTION_H_
#define _EASYIMAGE_BOUNDARY_EXTRACTION_H_

#include <drag2d.h>

namespace eimage
{

class BoundaryExtraction
{
public:
	BoundaryExtraction(const d2d::Image& image);

	void GetBoundaryLine(std::vector<d2d::Vector>& border) const;
	void GetBoundaryPoints(std::vector<d2d::Vector>& border) const;

private:
	bool IsPixelBorder(int x, int y) const;

	bool IsPixelTransparente(int x, int y) const;
	bool IsPixelTransparente(bool* flag, int x, int y) const;

	bool IsNearby(const d2d::Vector& p0, const d2d::Vector& p1) const;

private:
	const byte* m_pixels;
	int m_width, m_height;

}; // BoundaryExtraction

}

#endif // _EASYIMAGE_BOUNDARY_EXTRACTION_H_
