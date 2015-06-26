#include "StageCanvas.h"
#include "StagePanel.h"

#include "dataset/Grids.h"
#include "frame/config.h"
#include "frame/SettingCfg.h"

namespace lr
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::OnDraw()
{
	d2d::Rect sr = m_screen.GetRegion();

	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	m_stage->traverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);
	shader_mgr->SetSpriteShader(0);
	shader_mgr->sprite();

	m_stage->traverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);

	DrawRegion();

	if (const Grids* grids = m_stage->GetGrids()) {
		grids->Draw();
	}

	m_stage->drawEditTemp();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawRegion() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		cfg->m_map_width * 0.5f,
		cfg->m_map_height * 0.5f,
		d2d::LIGHT_GREY_LINE);

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		cfg->m_view_width * 0.5f,
		cfg->m_view_height * 0.5f,
		d2d::LIGHT_RED_LINE);

	m_stage->DebugDraw();
}

}