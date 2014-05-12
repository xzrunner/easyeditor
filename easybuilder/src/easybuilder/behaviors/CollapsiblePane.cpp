
#include "CollapsiblePane.h"

using namespace ebuilder;

CollapsiblePane::CollapsiblePane(wxWindow* parent, const wxString& name)
	: wxCollapsiblePane(parent, wxID_ANY, name)
{
	m_parent = parent;
//	SetBackgroundColour(*wxYELLOW);
}

void CollapsiblePane::Collapse(bool collapse/* = true*/)
{
	wxCollapsiblePane::Collapse(collapse);
	refresh();
}

void CollapsiblePane::refresh()
{
	m_parent->Layout();
	m_parent->Refresh();
	if (m_parent->GetParent())
	{
		CollapsiblePane* grand = dynamic_cast<CollapsiblePane*>(m_parent->GetParent());
		if (grand) grand->refresh();
	}
}
