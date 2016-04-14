#include "StageCanvas.h"
#include "StagePanel.h"
#include "typedef.h"

#include "dataset/Grids.h"
#include "frame/config.h"
#include "frame/SettingCfg.h"

#include <ee/DrawShapesVisitor.h>
#include <ee/EE_DTex.h>
#include <ee/Config.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/ShaderMgr.h>
#include <ee/EE_GTxt.h>
#include <ee/SettingData.h>
#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/EE_RVG.h>
#include <ee/color_config.h>
#include <ee/ScreenCache.h>
#include <ee/cfg_const.h>

#include <easyparticle3d.h>
#include <easytext.h>

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
	ee::Rect sr = m_screen.GetRegion();

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

		const std::string& tag = spr->tag;
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
}

void StageCanvas::DrawSprite(ee::Sprite* spr, bool draw_edge) const
{
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ee::Rect screen_region = m_screen.GetRegion();
		if (screen_region.IsValid() &&
			!ee::Math2D::IsRectIntersectRect(spr->GetRect(), screen_region)) {
			return;
		}
	}

	ee::SpriteRenderer* rd = ee::SpriteRenderer::Instance();

	int filter_mode_idx = 0;
	if (draw_edge) {
		filter_mode_idx = ee::FilterModes::Instance()->QueryShaderIdx(ee::FilterMode::FM_EDGE_DETECTION);
	}
//	ee::ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	rd->Draw(spr);

	ee::SettingData& cfg = ee::Config::Instance()->GetSettings();
	if (cfg.visible_node_name && !spr->name.empty() && spr->name[0] != '_') {
		ee::Matrix t;
		spr->GetTransMatrix(t);
		float s = std::max(1.0f, ee::CameraMgr::Instance()->GetCamera()->GetScale()) * cfg.node_name_scale;
		t.Scale(s, s);
		ee::GTxt::Instance()->Draw(t, spr->name);
	}
}

void StageCanvas::DrawRegion() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	ee::RVG::LineWidth(2);

	ee::RVG::Color(ee::LIGHT_GREY);
	ee::RVG::Rect(ee::Vector(0, 0), cfg->m_map_width * 0.5f, cfg->m_map_height * 0.5f, false);

	ee::RVG::Color(ee::LIGHT_RED);
	ee::RVG::Rect(ee::Vector(cfg->m_view_dx, cfg->m_view_dy), cfg->m_view_width * 0.5f, cfg->m_view_height * 0.5f, false);

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
//		ee::Vector center_world(0, 0);
//		ee::Vector center_screen = cam->TransPosProjectToScreen(center_world, w, h);
		ee::Vector center_screen(w * 0.5f, h * 0.5f);

		m_bound_pseudo3d.clear();
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x - ee::HALF_SCREEN_HEIGHT, center_screen.y - ee::HALF_SCREEN_WIDTH, w, h));
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x - ee::HALF_SCREEN_HEIGHT, center_screen.y + ee::HALF_SCREEN_WIDTH, w, h));
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x + ee::HALF_SCREEN_HEIGHT, center_screen.y + ee::HALF_SCREEN_WIDTH, w, h));
		m_bound_pseudo3d.push_back(cam->TransPosScreenToProject(center_screen.x + ee::HALF_SCREEN_HEIGHT, center_screen.y - ee::HALF_SCREEN_WIDTH, w, h));
	}

	if (!m_bound_pseudo3d.empty()) {
		ee::RVG::Color(ee::LIGHT_GREEN);
		ee::RVG::Polyline(m_bound_pseudo3d, true);
	}
}

}