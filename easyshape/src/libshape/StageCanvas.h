#ifndef _LIBSHAPE_STAGE_CANVAS_H_
#define _LIBSHAPE_STAGE_CANVAS_H_

#include <drag2d.h>

namespace libshape
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);
	StageCanvas(StagePanel* stage, d2d::ISprite* edited,
		const std::vector<d2d::ISprite*>& bg_sprites);

public:
	void drawGuideLines();

protected:
	virtual void initGL();
	virtual void onDraw();

	void onTimer(wxTimerEvent& event);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	d2d::ISprite* m_edited;
	std::vector<d2d::ISprite*> m_bg_sprites;

	d2d::MultiShapesImpl* m_stage_impl;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _LIBSHAPE_STAGE_CANVAS_H_