
#ifndef EMODELING_PREVIEW_CANVAS_H
#define EMODELING_PREVIEW_CANVAS_H

#include <drag2d.h>

namespace emodeling
{
	class PreviewPanel;

	class PreviewCanvas : public d2d::OrthoCanvas
	{
	public:
		PreviewCanvas(PreviewPanel* editPanel);
		virtual ~PreviewCanvas();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		enum
		{
			TIMER_ID = 1000
		};

		static const int FRAME_RATE = 60;

	private:
		wxTimer m_timer;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

#endif // EMODELING_PREVIEW_CANVAS_H
