#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"

#include "dataset/ISprite.h"

namespace d2d
{

void DrawSpritesVisitor::visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;

	ISprite* spr = static_cast<ISprite*>(object);
	if (!m_screen_region.isValid() || 
		Math::isRectIntersectRect(spr->GetRect(), m_screen_region)) {
		SpriteRenderer::Instance()->Draw(spr);
	}
}

} // d2d