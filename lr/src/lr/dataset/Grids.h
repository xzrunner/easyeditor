#ifndef _LR_GRIDS_H_
#define _LR_GRIDS_H_

#include <SM_Rect.h>
#include <cu/cu_stl.h>

#include <vector>

namespace lr
{

class Grids
{
public:
	Grids() {}

	void Draw() const;
	void Build(int width, int height);

	std::vector<int> IntersectPolygon(const CU_VEC<sm::vec2>& poly) const;
	std::vector<int> IntersectPolyline(const CU_VEC<sm::vec2>& path) const;

	void GetGridSize(int& col, int& row) {
		col = m_col;
		row = m_row;
	}

	// for debug
	void SetDebbugDrawGrids(const std::vector<int>& grids) {
		copy(grids.begin(), grids.end(), back_inserter(m_debug_draw_grids));
	}

private:
	static sm::vec2 TransToBirdView(float x, float y);
	static sm::vec2 TransToFlatView(float x, float y);

	void GetGridRegion(const CU_VEC<sm::vec2>& area, int& xmin, int& xmax, int& ymin, int& ymax) const;

	static void TransVerticesToFlat(const CU_VEC<sm::vec2>& src, CU_VEC<sm::vec2>& dst);

public:
	static const float EDGE;

private:
	static const float PROJ_TRANS;
	static const float BIRD_HW, BIRD_HH;

	struct Grid
	{
		Grid(float left, float top);

		// top left bottom right
		CU_VEC<sm::vec2> m_bird_bound;
		sm::rect m_flat_bound;
	}; // Grid

private:
	int m_width, m_height;
	int m_col, m_row;
	std::vector<Grid> m_grids;

	std::vector<sm::vec2> m_flat_bound;

	std::vector<int> m_debug_draw_grids;

}; // Grids

}

#endif // _LR_GRIDS_H_