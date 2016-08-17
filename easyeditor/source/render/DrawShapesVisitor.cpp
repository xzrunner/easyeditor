#include "DrawShapesVisitor.h"
#include "Shape.h"

#include <SM_Test.h>

namespace ee
{

DrawShapesVisitor::DrawShapesVisitor(const sm::rect& screen_region)
	: m_screen_region(screen_region)
{
}

void DrawShapesVisitor::Visit(Shape* shape, bool& next)
{
	next = true;

	if (!shape) {
		return;
	}

	if (!m_screen_region.IsValid() || 
		sm::is_rect_intersect_rect(shape->GetBounding(), m_screen_region)) {
		shape->Draw(sm::mat4(), m_ct);
	}
}

}