#ifndef EPACKER_STAGE_CANVAS_H
#define EPACKER_STAGE_CANVAS_H

#include <drag2d.h>

namespace epacker
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* stage);

	protected:
		virtual void onDraw();

	private:
		void onTimer(wxTimerEvent& event);

		void drawRegion();

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		d2d::MultiSpritesImpl* m_stage_impl;

		wxTimer m_timer;

		DECLARE_EVENT_TABLE()

	}; // StageCanvas
}

#endif // EPACKER_STAGE_CANVAS_H