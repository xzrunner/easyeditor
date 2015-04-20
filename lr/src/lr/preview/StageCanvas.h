#ifndef _LR_PREVIEW_STAGE_CANVAS_H_
#define _LR_PREVIEW_STAGE_CANVAS_H_

#include <drag2d.h>

namespace lr
{
namespace preview
{

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
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

private:
	wxTimer m_timer;
	d2d::PlayControl& m_control;

	std::vector<const d2d::ISprite*> m_sprites;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}
}

#endif // _LR_PREVIEW_STAGE_CANVAS_H_