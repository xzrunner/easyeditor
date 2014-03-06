#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class ToolbarPanel : public wxPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, Symbol* symbol);

	private:
		void initLayout();

		void onChangeSize(wxSpinEvent& event);

	private:
		Symbol* m_symbol;

		wxSpinCtrl *m_wSpin, *m_hSpin;

	}; // ToolbarPanel
}