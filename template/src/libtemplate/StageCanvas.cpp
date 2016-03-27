#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Camera.h>
#include <ee/DrawSpritesVisitor.h>

namespace etemplate
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	ee::Rect sr = m_screen.GetRegion();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

}