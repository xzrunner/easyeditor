#pragma once

#include <wx/wx.h>

namespace eanim
{
	class LayersContentWidget : public wxPanel
	{
	public:
		LayersContentWidget(wxWindow* parent);

		virtual wxCoord OnGetRowHeight(size_t row) const;

		virtual int GetNonOrientationTargetSize() const;
		virtual wxOrientation GetOrientation() const;

		void onSize(wxSizeEvent& event);
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);
		void onMouse(wxMouseEvent& event);

	private:
		static const int DRAG_FLAG_SIZE = 3;
		static const int DRAG_FLAG_RADIUS = 4;

	private:
		int m_dragFlagLine;

		DECLARE_EVENT_TABLE()

	}; // LayersContentWidget
}

