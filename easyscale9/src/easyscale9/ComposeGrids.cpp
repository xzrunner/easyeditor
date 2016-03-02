#include "ComposeGrids.h"

#include <ee/EE_RVG.h>
#include <ee/color_config.h>

namespace escale9
{

static const float EDGE = 100;
static const float X = EDGE * -1.5f;
static const float Y = EDGE * -1.5f;

void ComposeGrids::Draw()
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			ee::Vector p0;
			p0.x = X + EDGE*i;
			p0.y = Y + EDGE*j;
			ee::Vector p1(p0.x + EDGE, p0.y + EDGE);
			ee::RVG::Color(ee::LIGHT_GREY);
			ee::RVG::Rect(p0, p1, false);
		}
	}
}

void ComposeGrids::Query(const ee::Vector& pos, int* col, int* row)
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

ee::Vector ComposeGrids::GetGridCenter(int col, int row)
{
	return ee::Vector(X + (col + 0.5f) * EDGE, Y + (row + 0.5f) * EDGE);
}

}