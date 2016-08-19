#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/Camera.h>
#include <ee/CameraMgr.h>

#include <sprite2/S2_RVG.h>

namespace eui
{
namespace list
{

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), glctx, false)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), scale), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	s2::RVG::SetColor(s2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);
}

}
}