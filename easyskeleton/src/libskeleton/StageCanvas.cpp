#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/DrawSpritesVisitor.h>

#include <sprite2/S2_RVG.h>

namespace eskeleton
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), scale), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	s2::RVG::SetColor(s2::Color(204, 204, 204));
	s2::RVG::LineWidth(2);
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);
}

}