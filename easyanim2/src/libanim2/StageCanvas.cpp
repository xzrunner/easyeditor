#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/DrawSpritesVisitor.h>

namespace libanim2
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), gum::CAM_ORTHO2D)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);
	m_stage->DrawEditOP();
}

}