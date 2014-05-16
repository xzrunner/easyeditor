#ifndef _EASYPARTICLE2D_STAGE_CANVAS_H_
#define _EASYPARTICLE2D_STAGE_CANVAS_H_

#include <drag2d.h>

namespace eparticle2d
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
	clock_t m_last;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYPARTICLE2D_STAGE_CANVAS_H_
