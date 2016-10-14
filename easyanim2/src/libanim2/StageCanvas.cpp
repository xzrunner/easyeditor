#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/DrawSpritesVisitor.h>

namespace libanim2
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), scale), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

}