#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/DrawSpritesVisitor.h>

#include <sprite2/CameraType.h>

namespace libanim2
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);
	m_stage->DrawEditOP();
}

}