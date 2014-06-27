#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
{
}

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	if (d2d::Image* image = m_panel->getSprite()->getImage()) {
		image->reload();
	}
}

void StageCanvas::onDraw()
{
	const d2d::ISprite* bg = m_panel->GetBackground();
	if (bg) {
		d2d::SpriteDraw::drawSprite(bg);
	}
	m_editPanel->drawEditTemp();
}

}