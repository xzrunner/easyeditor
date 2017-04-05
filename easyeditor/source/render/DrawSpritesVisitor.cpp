#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "SettingData.h"
#include "Config.h"
#include "FilterModes.h"
#include "CurrSprTreePath.h"

#include <sprite2/DrawNode.h>
#include <gum/FilterModes.h>
#include <gum/GUM_GTxt.h>
#include <SM_Test.h>

namespace ee
{

void DrawSpritesVisitor::Visit(Sprite* spr, bool& next)
{
	next = true;

	if (!spr) {
		return;
	}

	s2::RenderParams params;
	params.view_region = m_screen_region;
	const s2::Actor* prev_actor = CurrSprTreePath::Instance()->TopActor();
	params.actor = spr->QueryActor(prev_actor);
	if (s2::DrawNode::IsOutsideView(spr, params)) {
		return;
	}

	SpriteRenderer::Instance()->Draw(spr, params);

	SettingData& cfg = Config::Instance()->GetSettings();
	const std::string& name = spr->GetName();
	if (cfg.visible_node_name && !name.empty() && name[0] != '_') 
	{
		sm::mat4 t = spr->GetLocalMat();
		float s = std::max(1.0f, m_cam_scale) * cfg.node_name_scale;
		t.Scale(s, s, 1);
		gum::GTxt::Instance()->Draw(t, name);
	}
}

}