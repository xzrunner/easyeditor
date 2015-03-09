#include "DrawShapesVisitor.h"

#include "dataset/IShape.h"
#include "common/Matrix.h"

namespace d2d
{

DrawShapesVisitor::DrawShapesVisitor(const Colorf& color/* = Colorf(0, 0, 0)*/)
	: m_color(color)
{
}

void DrawShapesVisitor::visit(Object* object, bool& bFetchNext)
{
	IShape* shape = static_cast<IShape*>(object);
	shape->draw(d2d::Matrix(), m_color);
	bFetchNext = true;
}

} // d2d