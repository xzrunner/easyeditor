#pragma once

#include <drag2d.h>

namespace eanim
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(d2d::EditPanel* stage);
		virtual ~StageCanvas();

		void setBackground(d2d::Image* image) {
			m_background = image;
		}

	protected:
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

		void onMouse(wxMouseEvent& event);
		void onKeyDown(wxKeyEvent& event);

	private:
		void drawbackground() const;

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		wxTimer m_timer;

		d2d::Image* m_background;

		DECLARE_EVENT_TABLE()

	}; // StageCanvas
}

