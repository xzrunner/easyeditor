
#ifndef EMODELING_FIXTURE_PROPERTY_SETTING_H
#define EMODELING_FIXTURE_PROPERTY_SETTING_H

#include <drag2d.h>

namespace libmodeling { class Fixture; }

namespace emodeling
{
	class FixturePropertySetting : public d2d::IPropertySetting
	{
	public:
		FixturePropertySetting(d2d::EditPanel* editPanel, libmodeling::Fixture* fixture);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		libmodeling::Fixture* m_fixture;

	}; // FixturePropertySetting
}

#endif // EMODELING_FIXTURE_PROPERTY_SETTING_H
