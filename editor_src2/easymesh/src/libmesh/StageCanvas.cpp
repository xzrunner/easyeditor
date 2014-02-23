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

void StageCanvas::onDraw()
{
	if (m_panel->m_sprite)
		d2d::SpriteDraw::drawSprite(m_panel->m_sprite);
}

}