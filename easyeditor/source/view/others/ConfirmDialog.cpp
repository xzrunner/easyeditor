#include "ConfirmDialog.h"

namespace ee
{

BEGIN_EVENT_TABLE(ConfirmDialog, wxDialog)
	EVT_CLOSE(ConfirmDialog::OnCloseEvent)
END_EVENT_TABLE()

ConfirmDialog::ConfirmDialog(wxWindow* parent)
	: BaseDialog(parent, "Save?")
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxButton(this, wxID_YES), 0, wxALL, 5);
	Connect(wxID_YES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ConfirmDialog::OnBtnYes));

	sizer->Add(new wxButton(this, wxID_NO), 0, wxALL, 5);
	Connect(wxID_NO, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ConfirmDialog::OnBtnNo));

	sizer->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);

	SetSizer(sizer);
	sizer->Fit(this);
}

void ConfirmDialog::OnBtnYes(wxCommandEvent& event)
{
	OnClose(true, wxID_YES);
}

void ConfirmDialog::OnBtnNo(wxCommandEvent& event)
{
	OnClose(true, wxID_NO);
}

void ConfirmDialog::OnCloseEvent(wxCloseEvent& event)
{
	OnClose(!event.CanVeto(), wxID_CANCEL);
}

void ConfirmDialog::OnClose(bool force, int returncode)
{
	if(IsModal())
	{
		EndModal(returncode);
	}
	else
	{
		SetReturnCode(returncode);
		Hide();
	}

	if(force)
	{
		Destroy();
	}
}

}