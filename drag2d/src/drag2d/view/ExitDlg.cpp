#include "ExitDlg.h"

namespace d2d
{

BEGIN_EVENT_TABLE(ExitDlg, wxDialog)
	EVT_CLOSE(ExitDlg::onCloseEvent)
END_EVENT_TABLE()

ExitDlg::ExitDlg(wxWindow* parent)
	: BaseDialog(parent, "Save?")
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
	sizer->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);

	SetSizer(sizer);
	sizer->Fit(this);
}

void ExitDlg::onCloseEvent(wxCloseEvent& event)
{
	onClose(!event.CanVeto(), wxID_CANCEL);
}

void ExitDlg::onClose(bool force, int returncode)
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