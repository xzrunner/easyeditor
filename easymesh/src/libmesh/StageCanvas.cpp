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
	m_panel->GetSymbol()->reloadTexture();
	resetViewport();
}

void StageCanvas::onDraw()
{
	const d2d::ISprite* bg = m_panel->GetBackground();
	if (bg) {
		d2d::SpriteDraw::drawSprite(m_screen, bg);
	}

	m_editPanel->drawEditTemp(m_screen);

	m_panel->traverseShapes(d2d::DrawShapesVisitor(m_screen), d2d::e_visible);

#ifdef _DEBUG 
	d2d::DynamicTexAndFont::Instance()->DebugDraw(m_screen);
#endif
}

}