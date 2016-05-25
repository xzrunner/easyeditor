#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/DrawSpritesVisitor.h>

#include <sprite2/RenderParams.h>

namespace emask
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
// 	sm::rect sr = m_screen.GetRegion();
// 	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
// 	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, scale), ee::DT_VISIBLE);

	m_stage->GetSymbol()->Draw(s2::RenderParams(), NULL);

	m_stage->DrawEditOP();
}

}