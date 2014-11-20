#ifndef _EASYIMAGE_BOUNDARY_EXTRACTION_H_
#define _EASYIMAGE_BOUNDARY_EXTRACTION_H_

#include <drag2d.h>

namespace eimage
{

class BoundaryExtraction
{
public:
	BoundaryExtraction(const d2d::Image& image);

	void GetRawBoundaryLine(std::vector<d2d::Vector>& border) const;
	void GetRawBoundaryPoints(std::vector<d2d::Vector>& border) const;

	void GetFineBoundaryLine(const std::vector<d2d::Vector>& raw_border, 
		std::vector<d2d::Vector>& fine_border, float tolerance) const;

private:
	bool IsPixelBorder(int x, int y) const;

	bool IsPixelTransparente(int x, int y) const;
	bool IsPixelTransparente(bool* flag, int x, int y) const;

	bool IsNearby(const d2d::Vector& p0, const d2d::Vector& p1) const;

	// fine
	void RemoveOneNode(const std::vector<d2d::Vector>& fine_border, int idx,
		const std::vector<d2d::Vector>& raw_border, d2d::Vector& new0, 
		d2d::Vector& new1, float& decrease) const;
	void AddOneNode(const std::vector<d2d::Vector>& fine_border, int idx,
		const std::vector<d2d::Vector>& raw_border, 
		const std::vector<d2d::Vector>& raw_border_merged,
		float r_decrease,
		d2d::Vector& new_start, d2d::Vector& new_end, d2d::Vector& new_node, 
		float& decrease) const;
	bool IsSegmentLegal(const std::vector<d2d::Vector>& raw_border,
		const d2d::Vector& p0, const d2d::Vector& p1, const d2d::Vector& p2) const;
	bool IsSegmentLegalNew(const std::vector<d2d::Vector>& raw_border_merged,
		const d2d::Vector& s0, const d2d::Vector& e0, const d2d::Vector& s1, const d2d::Vector& e1) const;

	void MergeRawBorder(const std::vector<d2d::Vector>& src, std::vector<d2d::Vector>& dst) const;

private:
	const byte* m_pixels;
	int m_width, m_height;

}; // BoundaryExtraction

}

#endif // _EASYIMAGE_BOUNDARY_EXTRACTION_H_
