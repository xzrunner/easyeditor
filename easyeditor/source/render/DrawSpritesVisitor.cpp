#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Math2D.h"
#include "SettingData.h"
#include "Config.h"
#include "EE_GTxt.h"
#include "FilterModes.h"

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

	int filter_mode_idx = FilterModes::Instance()->QueryShaderIdx(spr->GetShader().filter);
//	ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	DrawSprite(spr);

	SettingData& cfg = Config::Instance()->GetSettings();
	if (cfg.visible_node_name && !spr->name.empty() && spr->name[0] != '_') {
		sm::mat4 t;
		spr->GetTransMatrix(t);
		float s = std::max(1.0f, m_cam_scale) * cfg.node_name_scale;
		t.Scale(s, s, 1);
		GTxt::Instance()->Draw(t, spr->name);
	}
}

void DrawSpritesVisitor::DrawSprite(Sprite* spr) const
{
	SpriteRenderer::Draw(spr);
}

}