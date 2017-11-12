#ifndef _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_
#define _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_

#include "Visitor.h"
#include "Shape.h"

namespace ee
{

class DrawSelectedShapeVisitor : public RefVisitor<Shape>
{
public:
	virtual void Visit(const ShapePtr& shape, bool& next) override;

}; // DrawSelectedShapeVisitor

}

#endif // _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_