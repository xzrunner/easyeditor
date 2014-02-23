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
	m_panel->getSprite()->getImage()->reload();
}

void StageCanvas::onDraw()
{
	m_editPanel->drawEditTemp();
}

}