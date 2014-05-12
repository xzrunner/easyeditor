#pragma once

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

		wxPropertyGrid* getPG() { return m_pg; }

		void setPropertySetting(IPropertySetting* setting);

		void onPropertyGridChange(wxPropertyGridEvent& event);
		void updatePropertyGrid();
		void enablePropertyGrid(bool bEnable);

	protected:
		wxPropertyGrid* m_pg;

		IPropertySetting* m_setting;

	}; // PropertySettingPanel
}

