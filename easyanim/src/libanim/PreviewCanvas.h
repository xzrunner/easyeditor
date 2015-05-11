#pragma once

#include <drag2d.h>
#include <wx/wx.h>

#include "Symbol.h"

namespace libanim
{

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	struct PlaySetting
	{
		bool isCirculate;
		bool isStop;

		PlaySetting()
		{
			isCirculate = true;
			isStop = false;
		}
	};

public:
	PreviewCanvas(d2d::EditPanel* stage, const Symbol* symbol);

	PlaySetting& getPlaySetting();

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

	PlaySetting m_setting;

	const Symbol* m_symbol;

	d2d::PlayControl m_control;

	DECLARE_EVENT_TABLE()

}; // PreviewCanvas

}

