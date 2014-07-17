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
// 	if (const d2d::ISprite* s = m_stage->getImage()) {
// 		d2d::SpriteDraw::drawSprite(s);
// 	}

	if (const d2d::ISprite* s = m_stage->getLeft()) {
		d2d::SpriteDraw::drawSprite(s);
	}
	if (const d2d::ISprite* s = m_stage->getRight()) {
		d2d::SpriteDraw::drawSprite(s);
	}

	m_editPanel->drawEditTemp();
}
	
}