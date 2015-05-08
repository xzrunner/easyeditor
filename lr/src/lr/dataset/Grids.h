#ifndef _LR_GRIDS_H_
#define _LR_GRIDS_H_

#include <drag2d.h>

namespace lr
{

class Grids
{
public:
	Grids() {}

	void Draw() const;
	void Build(int width, int height);

	std::vector<int> IntersectPolygon(const std::vector<d2d::Vector>& poly) const;
	std::vector<int> IntersectPolyline(const std::vector<d2d::Vector>& line) const;

private:
	static d2d::Vector TransToBirdView(float x, float y);
	static d2d::Vector TransToFlatView(float x, float y);

	static d2d::Rect GetRegion(const std::vector<d2d::Vector>& vertices);
	static void TransVerticesToFlat(const std::vector<d2d::Vector>& src, std::vector<d2d::Vector>& dst);

public:
	static const int EDGE = 48;

private:
	static const float PROJ_TRANS;
	static const float BIRD_HW, BIRD_HH;

	struct Grid
	{
		Grid(float left, float top);

		// top left bottom right
		std::vector<d2d::Vector> m_bird_bound;
		d2d::Rect m_flat_bound;
	}; // Grid

private:
	int m_width, m_height;
	int m_col, m_row;
	std::vector<Grid> m_grids;

	std::vector<d2d::Vector> m_flat_bound;

}; // Grids

}

#endif // _LR_GRIDS_H_