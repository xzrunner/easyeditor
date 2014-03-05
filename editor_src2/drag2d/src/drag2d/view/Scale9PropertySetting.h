#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class Scale9Symbol;

	class Scale9PropertySetting : public IPropertySetting
	{
	public:
		Scale9PropertySetting(EditPanel* editPanel, Scale9Symbol* symbol);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		Scale9Symbol* m_symbol;

	}; // Scale9PropertySetting
}