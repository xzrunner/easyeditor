#include "StageCanvas.h"
#include "StagePanel.h"
#include "SettingCfg.h"

namespace emap
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	d2d::Rect sr = m_screen.GetRegion();
	m_panel->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);
	m_panel->TraverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);

	m_stage->DrawEditOP();
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