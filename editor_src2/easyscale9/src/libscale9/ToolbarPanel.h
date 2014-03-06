#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace escale9
{
	class ToolbarPanel : public wxPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, d2d::Scale9Symbol* symbol);

	private:
		void initLayout();

		void onChangeSize(wxSpinEvent& event);

	private:
		d2d::Scale9Symbol* m_symbol;

		wxSpinCtrl *m_wSpin, *m_hSpin;

	}; // ToolbarPanel
}