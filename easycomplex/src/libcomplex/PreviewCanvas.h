#ifndef _EASYCOMPLEX_PREVIEW_CANVAS_H_
#define _EASYCOMPLEX_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace ecomplex
{

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	PreviewCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
		const std::vector<const d2d::ISprite*>& sprites);

protected:
	virtual void initGL();
	virtual void onDraw();

	void onTimer(wxTimerEvent& event);

private:
	void drawStageData();

//	void getCurrSprites(std::vector<d2d::ISprite*>& sprites) const;

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;
	d2d::PlayControl& m_control;

	std::vector<const d2d::ISprite*> m_sprites;

	DECLARE_EVENT_TABLE()

}; // PreviewCanvas

}

#endif // _EASYCOMPLEX_PREVIEW_CANVAS_H_