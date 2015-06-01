#include "StageCanvas.h"
#include "StagePanel.h"
#include "SettingCfg.h"

namespace emap
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::DynamicStageCanvas(panel)
	, m_panel(panel)
{
}

void StageCanvas::onDraw()
{
	DrawGuideLines();

	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::DT_VISIBLE);
	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);

	m_stage->drawEditTemp();
}

void StageCanvas::DrawGuideLines() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		cfg->m_view_width * 0.5f,
		cfg->m_view_height * 0.5f,
		d2d::LIGHT_GREY_LINE);
}

}