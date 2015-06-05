#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"

#include "dataset/ISprite.h"

namespace d2d
{

void DrawSpritesVisitor::visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;
	SpriteRenderer::Instance()->Draw(static_cast<ISprite*>(object));
}

} // d2d