#include "DrawShapesVisitor.h"

#include "dataset/IShape.h"
#include "common/Matrix.h"

namespace d2d
{

DrawShapesVisitor::DrawShapesVisitor(const Rect& screen_region,
									 const Colorf& color/* = Colorf(0, 0, 0)*/)
	: m_screen_region(screen_region)
{
	m_ct.multi = color;
}

void DrawShapesVisitor::Visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;

	IShape* shape = static_cast<IShape*>(object);
	if (!shape) {
		return;
	}

	if (!m_screen_region.isValid() || 
		Math::isRectIntersectRect(shape->GetRect(), m_screen_region)) {
		shape->Draw(d2d::Matrix(), m_ct);
	}
}

} // d2d