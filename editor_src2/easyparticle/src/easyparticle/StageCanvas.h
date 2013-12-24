
#ifndef EPARTICLE_STAGE_CANVAS_H
#define EPARTICLE_STAGE_CANVAS_H

#include <drag2d.h>

namespace eparticle
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);
		virtual ~StageCanvas();

	protected:
		virtual void onDraw();

	private:
		void onTimer(wxTimerEvent& event);

		enum
		{
			TIMER_ID = 1000
		};

		static const int FRAME_RATE = 60;

	private:
		StagePanel* m_stage;

		wxTimer m_timer;

		DECLARE_EVENT_TABLE()

	}; // StageCanvas
}

#endif // EPARTICLE_STAGE_CANVAS_H
