#include "StageCanvas.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

namespace ep3dinv
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	d2d::Rect sr = m_screen.GetRegion();
	m_stage->traverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);

	if (m_stage->m_ps) {
		m_stage->m_ps->Update(1.0f / 30);
		m_stage->m_ps->Draw(d2d::Matrix());
	}

	m_stage->DrawEditOP();
}

}