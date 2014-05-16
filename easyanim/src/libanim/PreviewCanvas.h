#pragma once

#include <drag2d.h>
#include <wx/wx.h>

#include "Symbol.h"
#include "PreviewControl.h"

namespace anim
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
	PreviewCanvas(d2d::EditPanel* stage, d2d::LibraryPanel* library,
		const Symbol* symbol);

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
	int m_currFrame;

	PlaySetting m_setting;

	d2d::LibraryPanel* m_library;

	const Symbol* m_symbol;

	PreviewControl m_control;

	DECLARE_EVENT_TABLE()

}; // PreviewCanvas

}

