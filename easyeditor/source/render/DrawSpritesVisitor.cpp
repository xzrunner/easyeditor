#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Math2D.h"
#include "SettingData.h"
#include "ShaderMgr.h"
#include "Config.h"

namespace ee
{

void DrawSpritesVisitor::Visit(Object* object, bool& next)
{
	next = true;

	Sprite* spr = static_cast<Sprite*>(object);
	if (!spr) {
		return;
	}

	if (m_screen_region.IsValid() &&
		!Math2D::IsRectIntersectRect(spr->GetRect(), m_screen_region)) {
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
		t.Scale(s, s);
		etext::GTxt::Instance()->Draw(t, spr->name);
	}
}

void DrawSpritesVisitor::DrawSprite(SpriteRenderer* rd, Sprite* spr) const
{
	rd->Draw(spr);
}

}