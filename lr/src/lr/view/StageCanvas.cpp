#include "StageCanvas.h"
#include "StagePanel.h"

#include "dataset/Grids.h"
#include "frame/config.h"
#include "frame/SettingCfg.h"

namespace lr
{

StageCanvas::StageCanvas(StagePanel* statge)
	: d2d::DynamicStageCanvas(statge)
	, m_statge(statge)
{
}

void StageCanvas::OnDraw()
{
	m_statge->traverseSprites(d2d::DrawSpritesVisitor(), d2d::DT_VISIBLE);
	m_statge->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);

	DrawRegion();

	if (const Grids* grids = m_statge->GetGrids()) {
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

	m_statge->DebugDraw();
}

}