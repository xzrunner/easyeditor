#include "ComposeGrids.h"

#include <drag2d.h>

namespace escale9
{

static const float EDGE = 100;
static const float X = EDGE * -1.5f;
static const float Y = EDGE * -1.5f;

void ComposeGrids::Draw()
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			d2d::Vector p0;
			p0.x = X + EDGE*i;
			p0.y = Y + EDGE*j;
			d2d::Vector p1(p0.x + EDGE, p0.y + EDGE);
			d2d::PrimitiveDraw::rect(p0, p1, d2d::LIGHT_GREY_LINE);
		}
	}
}

void ComposeGrids::Query(const d2d::Vector& pos, int* col, int* row)
{
	if (pos.x < X || pos.x > X + EDGE * 3 ||
		pos.y < Y || pos.y > Y + EDGE * 3) {
		*col = -1;
		*row = -1;
		return;
	}

	*col = (int)((pos.x - X) / EDGE);
	*row = (int)((pos.y - Y) / EDGE);
}

d2d::Vector ComposeGrids::GetGridCenter(int col, int row)
{
	return d2d::Vector(X + (col + 0.5f) * EDGE, Y + (row + 0.5f) * EDGE);
}

}