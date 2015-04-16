#ifndef _LR_STAGE_CANVAS_H_
#define _LR_STAGE_CANVAS_H_

#include "drag2d.h"

namespace lr
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* statge);

protected:
	virtual void onDraw();

	void onTimer(wxTimerEvent& event);

private:
	void DrawGuideLines() const;

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	StagePanel* m_statge;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _LR_STAGE_CANVAS_H_
