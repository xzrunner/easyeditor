#include "BaseDialog.h"

namespace d2d
{

BaseDialog::BaseDialog(wxWindow* parent, wxString title)
	: wxDialog(parent, wxID_ANY, title)
{
	toCenter();
}

void BaseDialog::toCenter()
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

}