#include "DrawSpritesVisitor.h"
#include "SpriteDraw.h"

#include "dataset/ISprite.h"

namespace d2d
{

void DrawSpritesVisitor::visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;
	SpriteDraw::drawSprite(static_cast<ISprite*>(object));
}

} // d2d