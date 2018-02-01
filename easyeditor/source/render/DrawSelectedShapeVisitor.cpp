#include "DrawSelectedShapeVisitor.h"
#include "Shape.h"

#include <sprite2/RenderParams.h>

namespace ee
{

void DrawSelectedShapeVisitor::Visit(const ShapePtr& shape, bool& next)
{
	s2::RenderParams rp;
	rp.col_common.mul = pt2::Color(255, 0, 0);
	shape->Draw(nullptr, rp);
	next = true;
}

}