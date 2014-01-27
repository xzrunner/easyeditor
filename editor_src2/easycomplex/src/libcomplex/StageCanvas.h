#pragma once

#include <drag2d.h>

namespace libcomplex
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		enum
		{
			TIMER_ID = 1000
		};

	protected:
		StagePanel* m_editPanel;

	private:
		wxTimer m_timer;
		int m_currFrame;

		d2d::ShapeStyle m_bgStyle, m_clipboxStyle;

		DECLARE_EVENT_TABLE()

	}; // StageCanvas
}

