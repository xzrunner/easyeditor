#pragma once

#include <wx/wx.h>


namespace escale9
{
	class Symbol;

	class ToolbarPanel : public wxPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, ee::EditPanel* stage,
			Symbol* symbol);

	private:
		void InitLayout();

		void onChangeSize(wxSpinEvent& event);

	private:
		ee::EditPanel* m_stage;

		Symbol* m_symbol;

		wxSpinCtrl *m_wSpin, *m_hSpin;

	}; // ToolbarPanel
}