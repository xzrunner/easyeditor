#include "StageCanvas.h"
#include "StagePanel.h"

namespace esketch
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::onDraw()
{
	if (const ee::ISprite* s = m_stage->getImage())
	{ 
		ee::SpriteDraw::drawSprite(s);
	}

	ee::PrimitiveDraw::cross(ee::Vector(0, 0), 100, 100, ee::Colorf(1, 0, 0));

	m_editPanel->drawEditTemp();
}
	
}