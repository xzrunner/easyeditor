#include "DrawSelectedShapeVisitor.h"
#include "Shape.h"
#include "Matrix.h"

namespace ee
{

void DrawSelectedShapeVisitor::Visit(Object* object, bool& next) 
{
	Shape* shape = static_cast<Shape*>(object);
	ColorTrans col;
	col.multi.Set(1, 0, 0);
	shape->Draw(Matrix(), col);
	next = true;
}

}