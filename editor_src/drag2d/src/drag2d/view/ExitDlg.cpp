#include "ExitDlg.h"

namespace d2d
{
	ExitDlg::ExitDlg(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, "Save?")
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
		sizer->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);

		SetSizer(sizer);
		sizer->Fit(this);
	}
}