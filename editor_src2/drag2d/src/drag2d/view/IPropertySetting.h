#pragma once

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

namespace d2d
{
	class EditPanel;
	class PropertySettingPanel;

	class IPropertySetting
	{
	public:
		IPropertySetting(d2d::EditPanel* editPanel, const wxString& type);
		virtual ~IPropertySetting() {}

		virtual void updatePanel(PropertySettingPanel* panel) = 0;

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value) = 0;
		virtual void updatePropertyGrid(PropertySettingPanel* panel) = 0;
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable) = 0;

	protected:
		wxString getPGType(wxPropertyGrid* pg) const;

	protected:
		static const wxString TYPE_NAME;

	protected:
		d2d::EditPanel* m_editPanel;

		wxString m_type;

	}; // IPropertySetting
}

