#include "StageCanvas.h"
#include "StagePanel.h"

namespace lr
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* statge)
	: d2d::OrthoCanvas(statge)
	, m_statge(statge)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas::onDraw()
{
	static bool inited = false;
	if (!inited) {
		m_statge->GetSymbolsCfg().LoadConfig();
		inited = true;
	}

	m_statge->traverseSprites(d2d::DrawSpritesVisitor(), d2d::e_visible);

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

}