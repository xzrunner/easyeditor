#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>

#include <sprite2/RVG.h>
#include <sprite2/CameraType.h>

namespace eimage
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	if (const ee::SprPtr s = m_stage->GetImage())
	{
		ee::SpriteRenderer::Instance()->Draw(s);
	}

	s2::RVG::SetColor(pt2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);

	m_stage->DrawEditOP();
}
	
}