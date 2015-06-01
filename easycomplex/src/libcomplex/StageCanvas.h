#ifndef _DRAG2D_STAGE_CANVAS_H_
#define _DRAG2D_STAGE_CANVAS_H_

#include <drag2d.h>

namespace ecomplex
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* editPanel);
	virtual ~StageCanvas();

	void SetBackground(d2d::ISymbol* symbol);

protected:
	virtual void initGL();
	virtual void onDraw();

	void onTimer(wxTimerEvent& event);

protected:
	StagePanel* m_stage;

private:
	void drawBackground() const;

	// todo rm, only UpdateSymbol
	void UpdateParticle2d(float dt);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	d2d::ShapeStyle m_bgStyle, m_clipboxStyle;

	d2d::ISymbol* m_background;

	d2d::FpsStat m_stat;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _DRAG2D_STAGE_CANVAS_H_