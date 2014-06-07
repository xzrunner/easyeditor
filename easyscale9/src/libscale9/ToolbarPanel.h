#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class ToolbarPanel : public wxPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage,
			Symbol* symbol);

	private:
		void initLayout();

		void onChangeSize(wxSpinEvent& event);

	private:
		d2d::EditPanel* m_stage;

		Symbol* m_symbol;

		wxSpinCtrl *m_wSpin, *m_hSpin;

	}; // ToolbarPanel
}