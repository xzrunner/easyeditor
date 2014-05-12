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

		void onMouse(wxMouseEvent& event);
		void onKeyDown(wxKeyEvent& event);

	private:
		void drawbackground() const;

	private:
		d2d::Image* m_background;

	}; // StageCanvas
}

