#include "DrawSelectedShapeVisitor.h"
#include "Shape.h"

namespace ee
{

void DrawSelectedShapeVisitor::Visit(Shape* shape, bool& next) 
{
	s2::RenderColor col;
	col.mul.FromFloat(1, 0, 0);
	shape->Draw(sm::mat4(), col);
	next = true;
}

}