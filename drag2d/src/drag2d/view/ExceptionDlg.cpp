#include "ExceptionDlg.h"

#include "common/Exception.h"

namespace d2d
{

ExceptionDlg::ExceptionDlg(wxWindow* parent, const Exception& exp)
	: BaseDialog(parent, "Exception", wxDefaultPosition, wxSize(600, 480))
{
	InitLayout(exp.what());
}

void ExceptionDlg::InitLayout(const char* msg)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, msg, 
		wxDefaultPosition, wxSize(600, 480), wxTE_READONLY | wxTE_MULTILINE);
	sizer->Add(text, 0, wxALIGN_CENTRE | wxALL, 5);

	sizer->Add(new wxButton(this, wxID_OK), wxALL, 5);

	SetSizer(sizer);
	sizer->Fit(this);
}

}