#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"

#include "common/Config.h"
#include "common/SettingData.h"
#include "dataset/ISprite.h"
#include "render/ShaderMgr.h"

namespace d2d
{

void DrawSpritesVisitor::Visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;

	ISprite* spr = static_cast<ISprite*>(object);
	if (!spr) {
		return;
	}

	if (m_screen_region.isValid() &&
		!Math::isRectIntersectRect(spr->GetRect(), m_screen_region)) {
		return;
	}

	SpriteRenderer* rd = SpriteRenderer::Instance();

	int filter_mode_idx = FilterModes::Instance()->QueryShaderIdx(spr->GetFilterMode());
	ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	rd->Draw(spr);

	if (Config::Instance()->GetSettings().visible_node_name) {
		Matrix t;
		spr->GetTransMatrix(t);
		rd->DrawName(spr->name, std::max(1.0f, m_cam_scale), t);
	}
}

} // d2d