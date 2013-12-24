#pragma once

#include <drag2d.h>

namespace eanim
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(d2d::EditPanel* stage);

	protected:
		virtual void onDraw();

		void onMouse(wxMouseEvent& event);
		void onKeyDown(wxKeyEvent& event);

	private:
		void drawbackground() const;

	}; // StageCanvas
}

