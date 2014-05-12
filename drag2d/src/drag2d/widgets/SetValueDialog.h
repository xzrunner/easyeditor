#pragma once
#include <wx/wx.h>

namespace d2d
{
	class SetValueDialog : public wxDialog
	{
	public:
		SetValueDialog(wxWindow* parent, const wxString& title, const wxString& oldVal, const wxPoint& pos);

		wxString getText() const;

	private:
		void initLayout(const wxString& oldVal);

	private:
		wxTextCtrl* m_textCtrl;

	}; // SetValueDialog
}

