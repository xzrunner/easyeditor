#pragma once

#include <drag2d.h>

namespace coceditor
{
namespace complex
{
	class StagePanel;

	class StageCanvas : public d2d::OrthoCanvas
	{
	public:
		StageCanvas(StagePanel* editPanel);

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onMouse(wxMouseEvent& event);
		void onKeyDown(wxKeyEvent& event);

	private:
		StagePanel* m_editPanel;

		DECLARE_EVENT_TABLE()

	}; // StageCanvas
}
}

