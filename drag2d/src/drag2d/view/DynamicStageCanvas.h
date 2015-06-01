#ifndef _DRAG2D_DYNAMIC_STAGE_CANVAS_H_
#define _DRAG2D_DYNAMIC_STAGE_CANVAS_H_

#include "OrthoCanvas.h"

#include <wx/timer.h>

namespace d2d
{

class DynamicStageCanvas : public OrthoCanvas
{
public:
	DynamicStageCanvas(EditPanel* stage);
	virtual ~DynamicStageCanvas();

protected:
	virtual void OnTimer() {}

private:
	void OnTimer(wxTimerEvent& event);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	DECLARE_EVENT_TABLE()

}; // DynamicStageCanvas

}

#endif // _DRAG2D_DYNAMIC_STAGE_CANVAS_H_