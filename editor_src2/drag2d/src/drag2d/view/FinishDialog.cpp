#include "FinishDialog.h"

namespace d2d
{

FinishDialog::FinishDialog(wxWindow* parent)
	: BaseDialog(parent, wxT("Finish!"))
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
	SetSizer(sizer);
	sizer->Fit(this);
}

}