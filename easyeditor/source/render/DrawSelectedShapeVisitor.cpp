#include "DrawSelectedShapeVisitor.h"
#include "Shape.h"

namespace ee
{

void DrawSelectedShapeVisitor::Visit(Object* object, bool& next) 
{
	Shape* shape = static_cast<Shape*>(object);
	RenderColor col;
	col.multi.Set(1, 0, 0);
	shape->Draw(sm::mat4(), col);
	next = true;
}

}