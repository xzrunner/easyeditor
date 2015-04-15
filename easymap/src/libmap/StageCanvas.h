#ifndef _EASYMAP_STAGE_CANVAS_H_
#define _EASYMAP_STAGE_CANVAS_H_

#include "drag2d.h"

namespace emap
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
	void DrawGuideLines() const;

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

#endif // _EASYMAP_STAGE_CANVAS_H_
