#pragma once

#include "PropertySetting.h"

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

#include <memory>

namespace enode3d
{

class PropertySetting;

class PropertySettingPanel : public wxPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

//	void SetDefaultPropertySetting();
	void SetPropertySetting(std::unique_ptr<PropertySetting>& setting);

	wxPropertyGrid& GetPropertyGrid() { return *m_pg; }

	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void UpdatePropertyGrid();
	void EnablePropertyGrid(bool enable);

	const std::string& GetType() const { return m_type; }

private:
	void InitLayout();

private:
	std::string m_type;

	wxPropertyGrid* m_pg = nullptr;

	std::unique_ptr<PropertySetting> m_setting = nullptr;

}; // PropertySettingPanel

}