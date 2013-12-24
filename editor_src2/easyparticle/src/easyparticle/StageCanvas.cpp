
#include "StageCanvas.h"
#include "StagePanel.h"

using namespace eparticle;

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
	, m_stage(editPanel)
	, m_timer(this, TIMER_ID)
{
	m_bgColor.set(0, 0, 0, 1);

	m_timer.Start(1000 / FRAME_RATE);
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::onDraw()
{
	if (m_stage->m_particle)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		m_stage->m_particle->draw(0, 0, 0, 1, 1, 0, 0, 0, 0);
		m_stage->m_particle->draw();
		glDisable(GL_BLEND);
	}
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	if (m_stage->m_particle)
		m_stage->m_particle->update(1.0f / FRAME_RATE);

	Refresh();
}