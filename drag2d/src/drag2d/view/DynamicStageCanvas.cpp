#include "DynamicStageCanvas.h"

namespace d2d
{

BEGIN_EVENT_TABLE(DynamicStageCanvas, OrthoCanvas)
	EVT_TIMER(TIMER_ID, DynamicStageCanvas::OnTimer)
END_EVENT_TABLE()

DynamicStageCanvas::DynamicStageCanvas(EditPanel* stage)
	: OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(1000 / 30);
}

DynamicStageCanvas::~DynamicStageCanvas()
{
	m_timer.Stop();
}

void DynamicStageCanvas::OnTimer(wxTimerEvent& event)
{
	OnTimer();
	Refresh();
}

}