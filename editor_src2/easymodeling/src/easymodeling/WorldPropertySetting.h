
#ifndef EMODELING_WORLD_PROPERTY_SETTING_H
#define EMODELING_WORLD_PROPERTY_SETTING_H

#include <drag2d.h>

namespace emodeling
{
	class WorldPropertySetting : public d2d::IPropertySetting
	{
	public:
		WorldPropertySetting(d2d::EditPanel* editPanel);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	}; // WorldPropertySetting
}

#endif // EMODELING_WORLD_PROPERTY_SETTING_H