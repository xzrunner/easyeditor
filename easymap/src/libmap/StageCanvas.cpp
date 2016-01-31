#include "StageCanvas.h"
#include "StagePanel.h"
#include "SettingCfg.h"

namespace emap
{

StageCanvas::StageCanvas(StagePanel* panel)
	: ee::OrthoCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	ee::Rect sr = m_screen.GetRegion();
	m_panel->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);
	m_panel->TraverseShapes(ee::DrawShapesVisitor(sr), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	ee::PrimitiveDraw::DrawRect(ee::Vector(0, 0), 
		cfg->m_view_width * 0.5f,
		cfg->m_view_height * 0.5f,
		ee::LIGHT_GREY_LINE);
}

}