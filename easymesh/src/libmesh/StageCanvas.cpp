#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace emesh
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	m_panel->GetSymbol()->reloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
	resetViewport();
}

void StageCanvas::onDraw()
{
	const d2d::ISprite* bg = m_panel->GetBackground();
	if (bg) {
		d2d::SpriteDraw::drawSprite(bg);
	}

	m_editPanel->drawEditTemp();

	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::e_visible);

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

}