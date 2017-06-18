#include "MainPanel.h"
#include "MainList.h"

namespace edb
{

MainPanel::MainPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_list(NULL)
{
	InitLayout();
}

void MainPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" Main"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);

	m_list = new MainList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

}