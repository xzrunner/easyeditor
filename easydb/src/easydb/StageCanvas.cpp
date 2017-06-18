#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/DrawShapesVisitor.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/Sprite.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/CameraType.h>

namespace edb
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage_panel(editPanel)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	m_stage_panel->TraverseShapes(ee::DrawShapesVisitor(sm::rect(), GetCameraScale()), ee::DT_VISIBLE);
	m_stage_panel->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);

	m_stage_panel->DrawEditOP();
}

}