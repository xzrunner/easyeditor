#ifndef _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_
#define _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_

#include "Visitor.h"
#include "Shape.h"

namespace ee
{

class DrawSelectedShapeVisitor : public Visitor<Shape>
{
public:
	virtual void Visit(Shape* shape, bool& next);

}; // DrawSelectedShapeVisitor

}

#endif // _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_