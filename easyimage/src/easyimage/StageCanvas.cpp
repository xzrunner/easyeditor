#include "StageCanvas.h"
#include "StagePanel.h"

namespace eimage
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::onDraw()
{
	if (const d2d::ISprite* s = m_stage->getImage())
	{
		d2d::SpriteDraw::drawSprite(s);
	}

	d2d::PrimitiveDraw::cross(m_screen, d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

	m_editPanel->drawEditTemp(m_screen);
}
	
}