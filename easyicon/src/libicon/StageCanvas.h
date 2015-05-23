#ifndef _EASYICON_STAGE_CANVAS_H_
#define _EASYICON_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eicon
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void onDraw();

	void onTimer(wxTimerEvent& event);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYICON_STAGE_CANVAS_H_
