#ifndef _ESHADER_STAGE_CANVAS_H_
#define _ESHADER_STAGE_CANVAS_H_

#include <drag2d.h>

namespace eshader
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	virtual ~StageCanvas();

	void OnMousePressed(const d2d::Vector& pos);

protected:
	virtual void onDraw();

private:
	void DrawBackground() const;
	void DrawSprites() const;

	void OnTimer(wxTimerEvent& event);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	StagePanel* m_stage;

	clock_t m_start_time;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _ESHADER_STAGE_CANVAS_H_