#ifndef _DRAG2D_PROPERTY_SETTING_PANEL_H_
#define _DRAG2D_PROPERTY_SETTING_PANEL_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

namespace d2d
{

class IPropertySetting;

class PropertySettingPanel : public wxWindow
{
public:
	PropertySettingPanel(wxWindow* parent);
	~PropertySettingPanel();

	wxPropertyGrid* GetPropertyGrid() { return m_pg; }

	void SetPropertySetting(IPropertySetting* setting);

	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void UpdatePropertyGrid();
	void EnablePropertyGrid(bool enable);

	const std::string& GetType() const { return m_type; }

protected:
	std::string m_type;

	wxPropertyGrid* m_pg;

	IPropertySetting* m_setting;

}; // PropertySettingPanel

}

#endif // _DRAG2D_PROPERTY_SETTING_PANEL_H_