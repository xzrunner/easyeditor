#include "StageCanvas.h"
#include "StagePanel.h"
#include "typedef.h"

#include "dataset/Grids.h"
#include "frame/config.h"
#include "frame/SettingCfg.h"

#include <easyparticle3d.h>
#include <easytext.h>

namespace lr
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	d2d::Rect sr = m_screen.GetRegion();

	DrawSprites();

	m_stage->TraverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);

	DrawRegion();

	if (const Grids* grids = m_stage->GetGrids()) {
		grids->Draw();
	}

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawSprites() const
{
	std::vector<d2d::Sprite*> cover_layer, top_layer;

	std::vector<d2d::Sprite*> all_sprites;
	m_stage->TraverseSprites(d2d::FetchAllVisitor<d2d::Sprite>(all_sprites), d2d::DT_VISIBLE);
	for (int i = 0, n = all_sprites.size(); i < n; ++i) {
		d2d::Sprite* spr = all_sprites[i];

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
	std::sort(cover_layer.begin(), cover_layer.end(), d2d::SpriteCmp(d2d::SpriteCmp::e_y_invert));
	for (int i = 0, n = cover_layer.size(); i < n; ++i) {
		d2d::Sprite* spr = cover_layer[i];
		DrawSprite(spr, draw_flag);
	}

	for (int i = 0, n = top_layer.size(); i < n; ++i) {
		d2d::Sprite* spr = top_layer[i];
		DrawSprite(spr, false);
	}
}

void StageCanvas::DrawSprite(d2d::Sprite* spr, bool draw_edge) const
{
	d2d::Rect screen_region = m_screen.GetRegion();
	if (screen_region.IsValid() &&
		!d2d::Math2D::IsRectIntersectRect(spr->GetRect(), screen_region)) {
			return;
	}

	d2d::SpriteRenderer* rd = d2d::SpriteRenderer::Instance();

	int filter_mode_idx = 0;
	if (draw_edge) {
		filter_mode_idx = d2d::FilterModes::Instance()->QueryShaderIdx(d2d::FilterMode::FM_EDGE_DETECTION);
	}
	d2d::ShaderMgr::Instance()->SetSpriteShader(filter_mode_idx);

	rd->Draw(spr);

	d2d::SettingData& cfg = d2d::Config::Instance()->GetSettings();
	if (cfg.visible_node_name && !spr->name.empty() && spr->name[0] != '_') {
		d2d::Matrix t;
		spr->GetTransMatrix(t);
		float s = std::max(1.0f, m_camera->GetScale()) * cfg.node_name_scale;
		t.Scale(s, s);
		etext::GTxt::Instance()->Draw(t, spr->name);
	}
}

void StageCanvas::DrawRegion() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	d2d::PrimitiveDraw::DrawRect(d2d::Vector(0, 0), 
		cfg->m_map_width * 0.5f,
		cfg->m_map_height * 0.5f,
		d2d::LIGHT_GREY_LINE);

	d2d::PrimitiveDraw::DrawRect(
		d2d::Vector(cfg->m_view_dx, cfg->m_view_dy), 
		cfg->m_view_width * 0.5f,
		cfg->m_view_height * 0.5f,
		d2d::LIGHT_RED_LINE);

	m_stage->DebugDraw();
}

}