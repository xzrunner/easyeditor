#include "SearchPanel.h"
#include "SearchList.h"

namespace edb
{

SearchPanel::SearchPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_list(NULL)
{
	InitLayout();
}

void SearchPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" Search"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);

	m_list = new SearchList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

}