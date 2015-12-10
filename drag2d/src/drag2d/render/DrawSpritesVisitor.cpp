#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"

#include "common/Config.h"
#include "common/SettingData.h"
#include "dataset/ISprite.h"
#include "render/ShaderMgr.h"

#include <easytext.h>

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

	DrawSprite(rd, spr);

	SettingData& cfg = Config::Instance()->GetSettings();
	if (cfg.visible_node_name && !spr->name.empty() && spr->name[0] != '_') {
		Matrix t;
		spr->GetTransMatrix(t);
		float s = std::max(1.0f, m_cam_scale) * cfg.node_name_scale;
		t.scale(s, s);
		etext::GTxt::Instance()->Draw(t, spr->name);
	}
}

void DrawSpritesVisitor::DrawSprite(SpriteRenderer* rd, ISprite* spr) const
{
	rd->Draw(spr);
}

} // d2d