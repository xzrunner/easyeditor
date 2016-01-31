#include "TopToolbarPanel.h"

namespace eui
{

TopToolbarPanel::TopToolbarPanel(wxWindow* parent)
	: wxPanel(parent)
{
	InitLayout();
}

int TopToolbarPanel::AddToolbar(ee::ToolbarPanel* toolbar)
{
	wxSizer* sizer = GetSizer();
	sizer->Add(toolbar);
	Layout();
	
	int idx = m_toolbars.size();
	m_toolbars.push_back(toolbar);
	return idx;
}

void TopToolbarPanel::EnableToolbar(int idx)
{
	wxSizer* sizer = GetSizer();
	for (int i = 0; i < m_toolbars.size(); ++i) {
		sizer->Show((size_t)i, i == idx);
	}
	Layout();
}

void TopToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
}

}