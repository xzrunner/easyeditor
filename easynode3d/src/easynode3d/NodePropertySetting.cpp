#include "NodePropertySetting.h"

namespace enode3d
{

NodePropertySetting::NodePropertySetting(const NodePtr& node)
	: PropertySetting("Node")
	, m_node(node)
{
}

void NodePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{

}

void NodePropertySetting::UpdateProperties(wxPropertyGrid& pg)
{

}

void NodePropertySetting::InitProperties(wxPropertyGrid& pg)
{

}

}