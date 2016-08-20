#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/SpriteRenderer.h>

#include <sprite2/S2_RVG.h>

namespace eimage
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	if (const ee::Sprite* s = m_stage->GetImage())
	{
		ee::SpriteRenderer::Instance()->Draw(s);
	}

	s2::RVG::SetColor(s2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);

	m_stage->DrawEditOP();
}
	
}