#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace lib9patch
{
	class ToolbarPanel : public wxPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, d2d::Patch9Symbol* symbol);

	private:
		void initLayout();

		void onChangeSize(wxSpinEvent& event);

	private:
		d2d::Patch9Symbol* m_symbol;

		wxSpinCtrl *m_wSpin, *m_hSpin;

	}; // ToolbarPanel
}