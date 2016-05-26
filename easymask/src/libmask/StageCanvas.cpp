#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/Config.h>
#include <ee/EE_DTex.h>

#include <sprite2/RenderParams.h>

namespace emask
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_mask_render(false)
{
}

void StageCanvas::OnDrawSprites() const
{
	if (m_mask_render) {
		m_stage->GetSymbol()->Draw(s2::RenderParams(), NULL);
	} else {
	 	sm::rect sr = m_screen.GetRegion();
	 	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	 	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, scale), ee::DT_VISIBLE);
	}

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif
}

}