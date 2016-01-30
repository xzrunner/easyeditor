#include "DrawShapesVisitor.h"
#include "Shape.h"
#include "Math2D.h"
#include "Matrix.h"

namespace ee
{

DrawShapesVisitor::DrawShapesVisitor(const Rect& screen_region)
	: m_screen_region(screen_region)
{
}

void DrawShapesVisitor::Visit(Object* object, bool& next)
{
	next = true;

	Shape* shape = static_cast<Shape*>(object);
	if (!shape) {
		return;
	}

	if (!m_screen_region.IsValid() || 
		Math2D::IsRectIntersectRect(shape->GetRect(), m_screen_region)) {
		shape->Draw(Matrix(), m_ct);
	}
}

}