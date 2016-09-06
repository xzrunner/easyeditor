#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "SettingData.h"
#include "Config.h"
#include "FilterModes.h"

#include <sprite2/RenderFilter.h>
#include <sprite2/BoundingBox.h>
#include <glue/FilterModes.h>
#include <glue/GLUE_GTxt.h>
#include <SM_Test.h>

namespace ee
{

void DrawSpritesVisitor::Visit(Sprite* spr, bool& next)
{
	next = true;

	if (!spr) {
		return;
	}

	if (m_screen_region.IsValid() &&
		!sm::is_rect_intersect_rect(spr->GetBounding()->GetSize(), m_screen_region)) {
		return;
	}

	int filter_mode_idx = glue::FilterModes::Instance()->QueryShaderIdx(spr->Shader().filter->GetMode());
//	ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	DrawSprite(spr);

	SettingData& cfg = Config::Instance()->GetSettings();
	const std::string& name = spr->GetName();
	if (cfg.visible_node_name && !name.empty() && name[0] != '_') 
	{
		sm::mat4 t = spr->GetTransMatrix();
		float s = std::max(1.0f, m_cam_scale) * cfg.node_name_scale;
		t.Scale(s, s, 1);
		glue::GTxt::Instance()->Draw(t, name);
	}
}

void DrawSpritesVisitor::DrawSprite(Sprite* spr) const
{
	SpriteRenderer::Instance()->Draw(spr);
}

}