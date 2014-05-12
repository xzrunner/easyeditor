#pragma once

#include "AbstractWidget.h"

namespace d2d
{
	class OrthoCanvas;

	class SelectDrawTypeWidget : public AbstractWidget
	{
	public:
		SelectDrawTypeWidget(wxWindow* parent, OrthoCanvas* canvas);

	private:
		void onSetDrawType(wxCommandEvent& event);

	private:
		OrthoCanvas* m_canvas;

	}; // SelectDrawTypeWidget
}

