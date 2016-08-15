#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Math2D.h"
#include "SettingData.h"
#include "Config.h"
#include "EE_GTxt.h"
#include "FilterModes.h"

#include <sprite2/RenderFilter.h>

namespace ee
{

void DrawSpritesVisitor::Visit(Sprite* spr, bool& next)
{
	next = true;

	if (!spr) {
		return;
	}

	if (m_screen_region.IsValid() &&
		!Math2D::IsRectIntersectRect(spr->GetRect(), m_screen_region)) {
		return;
	}

	int filter_mode_idx = FilterModes::Instance()->QueryShaderIdx(spr->GetShader().filter->GetMode());
//	ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	DrawSprite(spr);

	SettingData& cfg = Config::Instance()->GetSettings();
	const std::string& name = spr->GetName();
	if (cfg.visible_node_name && !name.empty() && name[0] != '_') 
	{
		sm::mat4 t = spr->GetTransMatrix();
		float s = std::max(1.0f, m_cam_scale) * cfg.node_name_scale;
		t.Scale(s, s, 1);
		GTxt::Instance()->Draw(t, name);
	}
}

void DrawSpritesVisitor::DrawSprite(Sprite* spr) const
{
	SpriteRenderer::Draw(spr);
}

}