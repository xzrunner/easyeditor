#ifndef _EASYTEMPLATE_STAGE_CANVAS_H_
#define _EASYTEMPLATE_STAGE_CANVAS_H_

#include "drag2d.h"

namespace etemplate
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* panel);

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

	StagePanel* m_panel;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYTEMPLATE_STAGE_CANVAS_H_