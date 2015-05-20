#ifndef _EASYUI_PREVIEW_CANVAS_H_
#define _EASYUI_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace eui
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
	enum
	{
		TIMER_ID = 1000
	};

	static const float VIEW_WIDTH;
	static const float VIEW_HEIGHT;

private:
	d2d::Matrix m_scale_mt;

	wxTimer m_timer;
	d2d::PlayControl& m_control;

	std::vector<const d2d::ISprite*> m_sprites;

	DECLARE_EVENT_TABLE()

}; // PreviewCanvas

}

#endif // _EASYUI_PREVIEW_CANVAS_H_