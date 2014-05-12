#pragma once

#include <wx/wx.h>

namespace eanim
{
	class KeysScaleWidget : public wxPanel
	{
	public:
		KeysScaleWidget(wxWindow* parent);

		void onSize(wxSizeEvent& event);
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);
		void onMouse(wxMouseEvent& event);

	private:
		int queryGridByPos(float x) const;

	private:
		static const int DIVISION_HEIGHT = 4;
		static const int TEXT_Y = 4;

		DECLARE_EVENT_TABLE()

	}; // KeysScaleWidget
}

