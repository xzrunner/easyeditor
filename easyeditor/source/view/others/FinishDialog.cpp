#include "FinishDialog.h"

namespace ee
{

FinishDialog::FinishDialog(wxWindow* parent)
	: BaseDialog(parent, "Finish!")
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
	SetSizer(sizer);
	sizer->Fit(this);
}

}