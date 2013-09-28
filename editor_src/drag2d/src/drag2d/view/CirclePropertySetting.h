#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class CircleShape;

	class CirclePropertySetting : public IPropertySetting
	{
	public:
		CirclePropertySetting(EditPanel* editPanel, CircleShape* circle);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	private:
		CircleShape* m_circle;

	}; // CirclePropertySetting
}

