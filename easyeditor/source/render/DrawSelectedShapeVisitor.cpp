#include "DrawSelectedShapeVisitor.h"
#include "Shape.h"

#include <sprite2/RenderParams.h>

namespace ee
{

void DrawSelectedShapeVisitor::Visit(Shape* shape, bool& next) 
{
	s2::RenderParams rp;
	rp.color.SetMul(s2::Color(255, 0, 0));
	shape->Draw(rp);
	next = true;
}

}