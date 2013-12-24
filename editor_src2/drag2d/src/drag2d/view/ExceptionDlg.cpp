#include "ExceptionDlg.h"

#include "common/Exception.h"

namespace d2d
{
	ExceptionDlg::ExceptionDlg(wxWindow* parent, const Exception& exp)
		: wxDialog(parent, wxID_ANY, "exception")
	{
		initLayout(exp.what());
	}

	void ExceptionDlg::initLayout(const char* msg)
	{
		wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

		wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, msg, 
			wxDefaultPosition, wxSize(300, 200), wxTE_READONLY);
		sizer->Add(text, 0, wxALIGN_CENTRE | wxALL, 5);

		sizer->Add(new wxButton(this, wxID_OK), wxALL, 5);

		SetSizer(sizer);
		sizer->Fit(this);
	}
}