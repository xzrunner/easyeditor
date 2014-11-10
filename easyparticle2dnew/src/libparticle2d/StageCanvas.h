#ifndef _EASYPARTICLE2D_STAGE_CANVAS_H_
#define _EASYPARTICLE2D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easyanim.h>

namespace eparticle2d
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

protected:
	virtual void onDraw();

private:
	void onTimer(wxTimerEvent& event);

	void DrawBackground() const;

private:
	enum
	{
		TIMER_ID = 1000
	};

	static const int FRAME_RATE = 60;

private:
	// todo rm, only UpdateSymbol
	void UpdateParticle2d();
	void UpdateAnimation();

private:
	StagePanel* m_stage;

	wxTimer m_timer;
	int m_currFrame;

	d2d::PlayControl m_control;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYPARTICLE2D_STAGE_CANVAS_H_
