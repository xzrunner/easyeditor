#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace eicon
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas::onDraw()
{
	Sprite sprite;
	sprite.setSymbol(&m_stage->GetSymbol());
	sprite.SetProcess(0.5f);

	m_stage->GetSymbol().draw(d2d::Matrix(), d2d::Colorf(1, 1, 1, 1), d2d::Colorf(0, 0, 0, 0),
		d2d::Colorf(1, 0, 0, 0), d2d::Colorf(0, 1, 0, 0), d2d::Colorf(0, 0, 1, 0), &sprite);

	m_stage->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

}