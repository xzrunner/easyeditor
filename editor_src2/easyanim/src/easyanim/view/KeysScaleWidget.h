#pragma once
#include <wx/wx.h>

namespace eanim
{
	class KeysPanel;

	class KeysScaleWidget : public wxPanel
	{
	public:
		KeysScaleWidget(KeysPanel* parent, int id);

		void onSize(wxSizeEvent& event);
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);
		void onMouse(wxMouseEvent& event);

	private:
		int queryGridByPos(float x) const;

	private:
		static const int DIVISION_HEIGHT = 4;
		static const int TEXT_Y = 4;

	private:
		KeysPanel* m_parent;

		DECLARE_EVENT_TABLE()

	}; // KeysScaleWidget
}

