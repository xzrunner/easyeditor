#include "DrawShapesVisitor.h"

#include "dataset/IShape.h"
#include "common/Matrix.h"

namespace d2d
{

DrawShapesVisitor::DrawShapesVisitor(const Rect& screen_region,
									 const Colorf& color/* = Colorf(0, 0, 0)*/)
	: m_screen_region(screen_region)
	, m_color(color)
{
}

void DrawShapesVisitor::visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;

	IShape* shape = static_cast<IShape*>(object);
	if (!m_screen_region.isValid() || 
		Math::isRectIntersectRect(shape->getRect(), m_screen_region)) {
		shape->draw(d2d::Matrix(), m_color);
	}
}

} // d2d