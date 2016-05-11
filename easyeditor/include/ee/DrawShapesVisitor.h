#ifndef _EASYEDITOR_DRAW_SHAPES_VISITOR_H_
#define _EASYEDITOR_DRAW_SHAPES_VISITOR_H_

#include "Visitor.h"
#include "Rect.h"

#include <sprite2/RenderColor.h>

namespace ee
{

class DrawShapesVisitor : public Visitor
{
public:
	DrawShapesVisitor(const Rect& screen_region);

	virtual void Visit(Object* object, bool& next);

private:
	Rect m_screen_region;

	s2::RenderColor m_ct;

}; // DrawShapesVisitor

}

#endif // _EASYEDITOR_DRAW_SHAPES_VISITOR_H_