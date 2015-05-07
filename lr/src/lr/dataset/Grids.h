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

	static d2d::Vector TransToBirdView(float x, float y);
	static d2d::Vector TransToFlatView(float x, float y);

private:
	static const float PROJ_TRANS;
	static const int EDGE = 48;
	static const float BIRD_HW, BIRD_HH;

	struct Grid
	{
		Grid(float left, float top);

		// top left bottom right
		std::vector<d2d::Vector> m_bird_bound;
		d2d::Rect m_flat_bound;
	}; // Grid

private:
	int m_col, m_row;
	std::vector<Grid> m_grids;

	std::vector<d2d::Vector> m_flat_bound;

}; // Grids

}

#endif // _LR_GRIDS_H_