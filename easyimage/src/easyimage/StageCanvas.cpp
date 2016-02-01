#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/SpriteRenderer.h>
#include <ee/PrimitiveDraw.h>

namespace eimage
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	if (const ee::Sprite* s = m_stage->GetImage())
	{
		ee::SpriteRenderer::Instance()->Draw(s);
	}

	ee::PrimitiveDraw::Cross(ee::Vector(0, 0), 100, 100, ee::Colorf(1, 0, 0));

	m_stage->DrawEditOP();
}
	
}