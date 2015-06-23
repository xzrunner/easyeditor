#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"

#include "common/Config.h"
#include "common/SettingData.h"
#include "dataset/ISprite.h"

namespace d2d
{

void DrawSpritesVisitor::visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;

	ISprite* spr = static_cast<ISprite*>(object);
	if (!spr) {
		return;
	}

	if (!m_screen_region.isValid() || 
		Math::isRectIntersectRect(spr->GetRect(), m_screen_region)) 
	{
		SpriteRenderer* rd = SpriteRenderer::Instance();
		rd->Draw(spr);
		if (Config::Instance()->GetSettings().visible_node_name) {
			Matrix t;
			spr->GetTransMatrix(t);
			rd->DrawName(spr->name, std::max(1.0f, m_cam_scale), t);
		}
	}
}

} // d2d