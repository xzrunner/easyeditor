#include "view/NodeCompPanel.h"

#include <wx/sizer.h>

namespace eone
{

BEGIN_EVENT_TABLE(NodeCompPanel, wxCollapsiblePane)
	EVT_COLLAPSIBLEPANE_CHANGED(ID_NODE_COMP_PANEL, NodeCompPanel::OnCollapsChanged)
END_EVENT_TABLE()

NodeCompPanel::NodeCompPanel(wxWindow* parent, const std::string& title)
	: wxCollapsiblePane(parent, ID_NODE_COMP_PANEL, title)
{
}

void NodeCompPanel::OnCollapsChanged(wxCollapsiblePaneEvent& event)
{
	m_parent->Layout();
}

}