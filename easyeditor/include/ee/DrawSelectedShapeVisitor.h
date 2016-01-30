#ifndef _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_
#define _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_

#include "Visitor.h"

namespace ee
{

class DrawSelectedShapeVisitor : public Visitor
{
public:
	virtual void Visit(Object* object, bool& next);

}; // DrawSelectedShapeVisitor

}

#endif // _EASYEDITOR_DRAW_SELECTED_SHAPE_VISITOR_H_