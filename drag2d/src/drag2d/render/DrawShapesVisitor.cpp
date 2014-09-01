#include "DrawShapesVisitor.h"

#include "dataset/IShape.h"

namespace d2d
{

DrawShapesVisitor::DrawShapesVisitor(const Screen& scr, const Colorf& color/* = Colorf(0, 0, 0)*/)
	: m_scr(scr)
	, m_color(color)
{
}

void DrawShapesVisitor::visit(Object* object, bool& bFetchNext)
{
	IShape* shape = static_cast<IShape*>(object);
	shape->draw(m_scr, m_color);
	bFetchNext = true;
}

} // d2d