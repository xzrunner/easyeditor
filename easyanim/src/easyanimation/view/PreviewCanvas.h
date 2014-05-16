#pragma once

#include <drag2d.h>
#include <wx/wx.h>

namespace anim { class PreviewControl; }

namespace eanim
{

struct PlaySettings;

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	PreviewCanvas(d2d::EditPanel* stage, const PlaySettings& settings,
		anim::PreviewControl& control);

protected:
	virtual void initGL();
	virtual void onDraw();

	void onTimer(wxTimerEvent& event);

private:
	void drawStageData();

	void getCurrSprites(std::vector<d2d::ISprite*>& sprites) const;

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;
	anim::PreviewControl& m_control;

	const PlaySettings& m_settings;

	DECLARE_EVENT_TABLE()

}; // PreviewCanvas

}

