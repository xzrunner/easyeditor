#include "StageCanvas.h"
#include "StagePanel.h"

namespace etemplate
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

void StageCanvas::onDraw()
{
	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::e_visible);
	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::e_visible);

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

}