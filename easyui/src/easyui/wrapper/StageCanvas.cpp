#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/DrawSpritesVisitor.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/CameraType.h>

namespace eui
{
namespace wrapper
{

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D, glctx, false)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	s2::RVG::SetColor(s2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);
}

}
}