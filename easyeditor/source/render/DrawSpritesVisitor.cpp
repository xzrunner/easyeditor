#include "DrawSpritesVisitor.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "SettingData.h"
#include "Config.h"
#include "FilterModes.h"
#include "CurrSprTreePath.h"

#include <sprite2/DrawNode.h>
#include <s2loader/FilterModes.h>
#include <gum/GTxt.h>
#include <SM_Test.h>

namespace ee
{

void DrawSpritesVisitor::Visit(const SprPtr& spr, bool& next)
{
	next = true;

	if (!spr) {
		return;
	}

	s2::RenderParams params;
	params.SetViewRegion(m_screen_region);
	auto prev_actor = CurrSprTreePath::Instance()->TopActor();
	params.actor = spr->QueryActor(prev_actor.get());
	if (s2::DrawNode::CullingTestOutside(spr.get(), params)) {
		return;
	}

	SpriteRenderer::Instance()->Draw(spr.get(), params);

	SettingData& cfg = Config::Instance()->GetSettings();
	CU_STR name;
	s2::SprNameMap::Instance()->IDToStr(spr->GetName(), name);
	if (cfg.visible_node_name && !name.empty() && name[0] != '_') 
	{
		S2_MAT t = spr->GetLocalMat();
		float s = std::max(1.0f, m_cam_scale) * cfg.node_name_scale;
		t.Scale(s, s);
		gum::GTxt::Instance()->Draw(t, name);
	}
}

}