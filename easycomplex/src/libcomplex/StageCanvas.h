#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);
		virtual ~StageCanvas();

		void setBackground(d2d::Image* image) {
			image->retain();
			m_background = image;
		}

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	protected:
		StagePanel* m_editPanel;

	private:
		void drawBackground() const;

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		wxTimer m_timer;

		d2d::ShapeStyle m_bgStyle, m_clipboxStyle;

		d2d::Image* m_background;

		d2d::FpsStat m_stat;

		DECLARE_EVENT_TABLE()

	}; // StageCanvas
}

