#pragma once

#include "PropertySetting.h"
#include "Node.h"

namespace enode3d
{

class NodePropertySetting : public PropertySetting
{
public:
	NodePropertySetting(const NodePtr& node);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid& pg) override;
	virtual void InitProperties(wxPropertyGrid& pg) override;

private:
	NodePtr m_node;

}; // NodePropertySetting

}