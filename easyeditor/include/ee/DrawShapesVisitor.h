#ifndef _EASYEDITOR_DRAW_SHAPES_VISITOR_H_
#define _EASYEDITOR_DRAW_SHAPES_VISITOR_H_

#include "Visitor.h"
#include "Shape.h"

#include <sprite2/RenderColor.h>
#include <SM_Rect.h>

namespace ee
{

class DrawShapesVisitor : public Visitor<Shape>
{
public:
	DrawShapesVisitor(const sm::rect& screen_region, float cam_scale);

	virtual void Visit(Shape* shape, bool& next);

private:
	sm::rect m_screen_region;
	float m_cam_scale;

	s2::RenderColor m_ct;

}; // DrawShapesVisitor

}

#endif // _EASYEDITOR_DRAW_SHAPES_VISITOR_H_