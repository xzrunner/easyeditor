#pragma once

#include <drag2d.h>
#include <wx/wx.h>

namespace eanim
{

class Controller;
struct PlaySettings;

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	PreviewCanvas(d2d::EditPanel* stage, const PlaySettings& settings,
		d2d::PlayControl& control, Controller* ctrl);

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
	d2d::PlayControl& m_control;

	const PlaySettings& m_settings;

	Controller* m_ctrl;

	DECLARE_EVENT_TABLE()

}; // PreviewCanvas

}

