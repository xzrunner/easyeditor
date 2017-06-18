#include "WarnPanel.h"
#include "WarnList.h"

namespace edb
{

WarnPanel::WarnPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_list(NULL)
{
	InitLayout();
}

void WarnPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" Warn"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);

	m_list = new WarnList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

}