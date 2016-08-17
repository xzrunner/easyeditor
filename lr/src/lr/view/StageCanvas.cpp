#include "StageCanvas.h"
#include "StagePanel.h"
#include "typedef.h"

#include "dataset/Grids.h"
#include "dataset/Layer.h"
#include "frame/config.h"
#include "frame/SettingCfg.h"

#include <ee/DrawShapesVisitor.h>
#include <ee/EE_DTex.h>
#include <ee/Config.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/EE_GTxt.h>
#include <ee/SettingData.h>
#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/color_config.h>
#include <ee/ScreenCache.h>
#include <ee/cfg_const.h>
#include <ee/Math2D.h>
#include <ee/FilterModes.h>

#include <easyparticle3d.h>
#include <easytext.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/BoundingBox.h>

#include <algorithm>

namespace lr
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	sm::rect sr = m_screen.GetRegion();

	DrawSprites();

	m_stage->TraverseShapes(ee::DrawShapesVisitor(sr), ee::DT_VISIBLE);

	DrawRegion();

	if (const Grids* grids = m_stage->GetGrids()) {
		grids->Draw();
	}

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawSprites() const
{
	std::vector<ee::Sprite*> cover_layer, top_layer;

	std::vector<ee::Sprite*> all_sprites;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(all_sprites), ee::DT_VISIBLE);
	for (int i = 0, n = all_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = all_sprites[i];

		const std::string& tag = spr->GetTag();
		if (tag.find(TOP_LAYER_TAG) != std::string::npos) {
			top_layer.push_back(spr);
		} else if (tag.find(COVER_LAYER_TAG) != std::string::npos
			|| dynamic_cast<eparticle3d::Sprite*>(spr)) {
			cover_layer.push_back(spr);
		} else {
			DrawSprite(spr, false);
		}
	}

	bool draw_flag = SettingCfg::Instance()->m_special_layer_flag;
	std::sort(cover_layer.begin(), cover_layer.end(), ee::SpriteCmp(ee::SpriteCmp::e_y_invert));
	for (int i = 0, n = cover_layer.size(); i < n; ++i) {
		ee::Sprite* spr = cover_layer[i];
		DrawSprite(spr, draw_flag);
	}

	for (int i = 0, n = top_layer.size(); i < n; ++i) {
		ee::Sprite* spr = top_layer[i];
		DrawSprite(spr, false);
	}

	// draw point layer again
	Layer* point_layer = m_stage->GetLayers()[3];
	std::vector<ee::Sprite*> redraw_sprites;
	point_layer->TraverseSprite(ee::FetchAllVisitor<ee::Sprite>(redraw_sprites), ee::DT_VISIBLE);
	for (int i = 0, n = redraw_sprites.size(); i < n; ++i) {
		DrawSprite(redraw_sprites[i], false);
	}
}

void StageCanvas::DrawSprite(ee::Sprite* spr, bool draw_edge) const
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		sm::rect screen_region = m_screen.GetRegion();
		if (screen_region.IsValid() &&
			!sm::is_rect_intersect_rect(spr->GetBounding()->GetSize(), screen_region)) {
			return;
		}
	}

	int filter_mode_idx = 0;
	if (draw_edge) {
		filter_mode_idx = ee::FilterModes::Instance()->QueryShaderIdx(s2::FilterMode::FM_EDGE_DETECTION);
	}
//	ee::ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	ee::SpriteRenderer::Draw(spr);

	ee::SettingData& cfg = ee::Config::Instance()->GetSettings();
	const std::string& name = spr->GetName();
	if (cfg.visible_node_name && !name.empty() && name[0] != '_') {
		sm::mat4 t = spr->GetTransMatrix();
		float s = std::max(1.0f, ee::CameraMgr::Instance()->GetCamera()->GetScale()) * cfg.node_name_scale;
		t.x[0] = t.x[5] = s;
		ee::GTxt::Instance()->Draw(t, name);
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
	if (!ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) 
	{
		ee::Camera* cam = static_cast<ee::Camera*>(ee::CameraMgr::Instance()->GetCamera());
		int w, h;
		ee::ScreenCache::Instance()->GetSize(w, h);
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

}