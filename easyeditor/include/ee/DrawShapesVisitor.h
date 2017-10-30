#ifndef _EASYEDITOR_DRAW_SHAPES_VISITOR_H_
#define _EASYEDITOR_DRAW_SHAPES_VISITOR_H_

#include "Visitor.h"
#include "Shape.h"

#include <sprite2/RenderParams.h>
#include <SM_Rect.h>

namespace ee
{

class DrawShapesVisitor : public RefVisitor<Shape>
{
public:
	DrawShapesVisitor(const sm::rect& screen_region, float cam_scale);

	virtual void Visit(const std::shared_ptr<Shape>& shape, bool& next);

private:
	sm::rect m_screen_region;
	float m_cam_scale;

	s2::RenderParams m_rp;

}; // DrawShapesVisitor

}

#endif // _EASYEDITOR_DRAW_SHAPES_VISITOR_H_