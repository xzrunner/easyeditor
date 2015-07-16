#include "BaseDialog.h"

namespace d2d
{

BEGIN_EVENT_TABLE(BaseDialog, wxDialog)
	EVT_CLOSE(BaseDialog::OnCloseEvent)
END_EVENT_TABLE()

BaseDialog::BaseDialog(wxWindow* parent, wxString title, const wxPoint& pos, const wxSize& size)
	: wxDialog(parent, wxID_ANY, title, pos, size)
{
	MoveToCenter();
}

void BaseDialog::MoveToCenter()
{
	wxRect rect;
	wxWindow* top = m_parent;
	do 
	{
		rect = top->GetRect();
		top = top->GetParent();
	} while (top);

	int x = rect.GetLeft() + rect.GetWidth() * 0.5f;
	int y = rect.GetTop() + rect.GetHeight() * 0.5f;
 	SetPosition(wxPoint(x, y));
}

void BaseDialog::OnCloseEvent(wxCloseEvent& event)
{
	OnClose(!event.CanVeto(), wxID_CANCEL);
}

void BaseDialog::OnClose(bool force, int returncode)
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