#include "DrawSelectedSpriteVisitor.h"
#include "PrimitiveDraw.h"

#include "dataset/AbstractBV.h"
#include "dataset/ISprite.h"

namespace d2d
{

DrawSelectedSpriteVisitor::DrawSelectedSpriteVisitor(const Colorf& color)
	: m_color(color)
{}

void DrawSelectedSpriteVisitor::visit(Object* object, bool& bFetchNext) 
{
	std::vector<Vector> bound;
	ISprite* sprite = static_cast<ISprite*>(object);
	sprite->getBounding()->getBoundPos(bound);

	PrimitiveDraw::drawPolyline(bound, m_color, true);

	bFetchNext = true;
}

} // d2d