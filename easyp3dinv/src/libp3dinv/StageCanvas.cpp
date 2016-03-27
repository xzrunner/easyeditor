#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/Camera.h>
#include <ee/CameraMgr.h>
#include <ee/Matrix.h>

namespace ep3dinv
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	ee::Rect sr = m_screen.GetRegion();
	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, scale), ee::DT_VISIBLE);

	if (m_stage->m_ps) {
		m_stage->m_ps->Update(1.0f / 30);
		m_stage->m_ps->Draw(ee::Matrix());
	}

	m_stage->DrawEditOP();
}

}