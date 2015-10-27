#include "ToolbarPanel.h"

namespace eui
{

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: wxPanel(parent)
{
	InitLayout();
	EnableToolbar(0);
}

int ToolbarPanel::AddToolbar(d2d::ToolbarPanel* toolbar)
{
	wxSizer* sizer = GetSizer();
	sizer->Add(toolbar);
	Layout();
	
	int idx = m_toolbars.size();
	m_toolbars.push_back(toolbar);
	return idx;
}

void ToolbarPanel::EnableToolbar(int idx)
{
	wxSizer* sizer = GetSizer();
	for (int i = 0; i < m_toolbars.size(); ++i) {
		sizer->Show((size_t)i, i == idx);
	}
	Layout();
}

void ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
}

}