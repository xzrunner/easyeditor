#pragma once

#include <string>

#include <wx/propgrid/propgrid.h>

namespace enode3d
{

class PropertySettingPanel;

class PropertySetting
{
public:
	PropertySetting(const std::string& type);
	virtual ~PropertySetting() {}

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) = 0;

	void UpdateFromPanel(PropertySettingPanel& panel);
	void EnablePropertyGrid(PropertySettingPanel& panel, bool enable);

	const std::string& GetType() const { return m_type; }

protected:
	virtual void UpdateProperties(wxPropertyGrid& pg) = 0;
	virtual void InitProperties(wxPropertyGrid& pg) = 0;

private:
	std::string m_type;

}; // PropertySetting

}