
#include "StageCanvas.h"
#include "StagePanel.h"

namespace eparticle
{
namespace coco
{

	BEGIN_EVENT_TABLE(StageCanvas, d2d::PerspectCanvas)
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
			if (m_stage->m_background)
				d2d::SpriteDraw::drawSprite(m_stage->m_background);

 			m_stage->m_particle->draw();
 		}
	}

	void StageCanvas::onTimer(wxTimerEvent& event)
	{
		if (m_stage->m_particle)
			m_stage->m_particle->update(1.0f / FRAME_RATE);

		Refresh();
	}

}
}