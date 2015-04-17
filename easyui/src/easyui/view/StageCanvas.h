#ifndef _EASYUI_STAGE_CANVAS_H_
#define _EASYUI_STAGE_CANVAS_H_

#include "drag2d.h"

namespace eui
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

#endif // _EASYUI_STAGE_CANVAS_H_
