#include "StageCanvas.h"
#include "StagePanel.h"
#include "typedef.h"

#include "dataset/Grids.h"
#include "dataset/Layer.h"
#include "frame/config.h"
#include "frame/SettingCfg.h"

#include <ee/DrawShapesVisitor.h>
#include <ee/Config.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/SettingData.h>
#include <ee/color_config.h>
#include <ee/cfg_const.h>
#include <ee/Math2D.h>
#include <ee/FilterModes.h>

#include <easyparticle3d.h>
#include <easytext.h>

#include <SM_Test.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/BoundingBox.h>
#include <sprite2/CameraType.h>
#include <sprite2/Camera.h>
#include <sprite2/Pseudo3DCamera.h>
#include <gum/FilterModes.h>
#include <gum/GUM_GTxt.h>
#include <gum/RenderContext.h>

#include <algorithm>

namespace lr
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawSprites();

	m_stage->TraverseShapes(ee::DrawShapesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);

	DrawRegion();

	if (const Grids* grids = m_stage->GetGrids()) {
		grids->Draw();
	}

	m_stage->DrawEditOP();
}

void StageCanvas::DrawSprites() const
{
	bool force_visible = SettingCfg::Instance()->m_all_layers_visible_editable;

	std::vector<DrawableSpr> cover_layer, top_layer;

	const std::vector<Layer*>& layers = m_stage->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		if (i == 3 || i == 8) {
			continue;
		}

		Layer* layer = layers[i];
		if (!force_visible && !layer->IsVisible()) {
			continue;
		}

		int name_visible = layer->GetNameVisible();

		std::vector<ee::Sprite*> sprs;
		layer->TraverseSprite(ee::FetchAllVisitor<ee::Sprite>(sprs), ee::DT_VISIBLE);
		for (int j = 0, m = sprs.size(); j < m; ++j) {
			ee::Sprite* spr = sprs[j];
			const std::string& tag = spr->GetTag();
			if (tag.find(TOP_LAYER_TAG) != std::string::npos) {
				top_layer.push_back(DrawableSpr(spr, name_visible));
			} else if (tag.find(COVER_LAYER_TAG) != std::string::npos) {
				cover_layer.push_back(DrawableSpr(spr, name_visible));
			} else {
				DrawSprite(spr, false, name_visible);
			}
		}		
	}

	bool draw_flag = SettingCfg::Instance()->m_special_layer_flag;
	std::sort(cover_layer.begin(), cover_layer.end(), SprCmp());
	for (int i = 0, n = cover_layer.size(); i < n; ++i) {
		DrawSprite(cover_layer[i].spr, draw_flag, cover_layer[i].name_visible);
	}
	for (int i = 0, n = top_layer.size(); i < n; ++i) {
		DrawSprite(top_layer[i].spr, false, top_layer[i].name_visible);
	}

	DrawLayer(layers[3]);
	DrawLayer(layers[8]);
}

void StageCanvas::DrawSprite(ee::Sprite* spr, bool draw_edge, int name_visible) const
{
	if (m_camera->Type() == s2::CAM_ORTHO2D)
	{
		sm::rect screen_region = GetVisibleRegion();
		if (screen_region.IsValid() &&
			!sm::is_rect_intersect_rect(spr->GetBounding()->GetSize(), screen_region)) {
			return;
		}		
	}

	int filter_mode_idx = 0;
	if (draw_edge) {
		filter_mode_idx = gum::FilterModes::Instance()->QueryShaderIdx(s2::FilterMode::FM_EDGE_DETECTION);
	}
//	ee::ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	ee::SpriteRenderer::Instance()->Draw(spr);

	ee::SettingData& cfg = ee::Config::Instance()->GetSettings();
	if ((name_visible == -1 && cfg.visible_node_name) || name_visible == 1) 
	{
		ee::SettingData& cfg = ee::Config::Instance()->GetSettings();
		const std::string& name = spr->GetName();
		if (cfg.visible_node_name && !name.empty() && name[0] != '_') 
		{
			S2_MAT t = spr->GetLocalMat();
			float s = std::max(1.0f, GetCameraScale()) * cfg.node_name_scale;
			t.x[0] = t.x[5] = s;
			gum::GTxt::Instance()->Draw(t, name);
		}
	}
}

void StageCanvas::DrawRegion() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	s2::RVG::LineWidth(2);

	s2::RVG::SetColor(ee::LIGHT_GREY);
	s2::RVG::Rect(sm::vec2(0, 0), cfg->m_map_width * 0.5f, cfg->m_map_height * 0.5f, false);

	s2::RVG::SetColor(ee::LIGHT_RED);
	s2::RVG::Rect(sm::vec2(cfg->m_view_dx, cfg->m_view_dy), cfg->m_view_width * 0.5f, cfg->m_view_height * 0.5f, false);

	DrawPseudo3dBound();

	m_stage->DebugDraw();
}

void StageCanvas::DrawPseudo3dBound() const
{
	if (m_camera->Type() == s2::CAM_PSEUDO3D)
	{
		s2::Pseudo3DCamera* cam = static_cast<s2::Pseudo3DCamera*>(m_camera);
		int w = gum::RenderContext::Instance()->GetWidth(),
			h = gum::RenderContext::Instance()->GetHeight();
//		sm::vec2 center_world(0, 0);
//		sm::vec2 center_screen = cam->TransPosProjectToScreen(center_world, w, h);
		sm::vec2 center_screen(w * 0.5f, h * 0.5f);

		m_bound_pseudo3d.clear();
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x - ee::HALF_SCREEN_HEIGHT, center_screen.y - ee::HALF_SCREEN_WIDTH, w, h));
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x - ee::HALF_SCREEN_HEIGHT, center_screen.y + ee::HALF_SCREEN_WIDTH, w, h));
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x + ee::HALF_SCREEN_HEIGHT, center_screen.y + ee::HALF_SCREEN_WIDTH, w, h));
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x + ee::HALF_SCREEN_HEIGHT, center_screen.y - ee::HALF_SCREEN_WIDTH, w, h));
	}

	if (!m_bound_pseudo3d.empty()) {
		s2::RVG::SetColor(ee::LIGHT_GREEN);
		s2::RVG::Polyline(m_bound_pseudo3d, true);
	}
}

void StageCanvas::DrawLayer(const Layer* layer) const
{
	std::vector<ee::Sprite*> sprs;
	layer->TraverseSprite(ee::FetchAllVisitor<ee::Sprite>(sprs), ee::DT_VISIBLE);
	int name_visible = layer->GetNameVisible();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		DrawSprite(sprs[i], false, name_visible);
	}
}

/************************************************************************/
/* class StageCanvas::SprCmp                                            */
/************************************************************************/

bool StageCanvas::SprCmp::
operator() (const DrawableSpr& s0, const DrawableSpr& s1) const
{
	return s0.spr->GetPosition().y > s1.spr->GetPosition().y;
}

}