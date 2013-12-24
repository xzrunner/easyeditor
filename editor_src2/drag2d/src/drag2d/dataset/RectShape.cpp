#include "RectShape.h"

#include "common/Math.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

RectShape::RectShape()
	: m_rect(Vector(0, 0), 1, 1)
{
}

RectShape::RectShape(const RectShape& rect)
	: m_rect(rect.m_rect)
{
}

RectShape::RectShape(const Vector& p0, const Vector& p1)
	: m_rect(p0, p1)
{
}

RectShape::RectShape(const Vector& center, float hWidth, float hHeight)
	: m_rect(center, hWidth, hHeight)
{
}

RectShape* RectShape::clone()
{
	return new RectShape(*this);
}

void RectShape::loadFromTextFile(std::ifstream& fin)
{
}

void RectShape::storeToTextFile(std::ofstream& fout) const
{
}


bool RectShape::isContain(const Vector& pos) const
{
	return Math::isPointInRect(pos, m_rect);
}

bool RectShape::isIntersect(const Rect& rect) const
{
	return Math::isRectIntersectRect(rect, m_rect);
}

void RectShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	PrimitiveDraw::drawRect(Vector(m_rect.xMin, m_rect.yMin), 
		Vector(m_rect.xMax, m_rect.yMax), false, 2, color);
}
} // d2d