#include "ee0/WxCompPanel.h"

#include <wx/sizer.h>

namespace ee0
{

BEGIN_EVENT_TABLE(WxCompPanel, wxCollapsiblePane)
	EVT_COLLAPSIBLEPANE_CHANGED(ID_NODE_COMP_PANEL, WxCompPanel::OnCollapsChanged)
END_EVENT_TABLE()

WxCompPanel::WxCompPanel(wxWindow* parent, const std::string& title)
	: wxCollapsiblePane(parent, ID_NODE_COMP_PANEL, title)
{
}

void WxCompPanel::OnCollapsChanged(wxCollapsiblePaneEvent& event)
{
	m_parent->Layout();
}

}