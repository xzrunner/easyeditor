
#ifndef EMODELING_BODY_PROPERTY_SETTING_H
#define EMODELING_BODY_PROPERTY_SETTING_H

#include <drag2d.h>

namespace libmodeling { class Body; }

namespace emodeling
{
	class BodyPropertySetting : public d2d::IPropertySetting
	{
	public:
		BodyPropertySetting(d2d::EditPanel* editPanel, d2d::ISprite* sprite);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		libmodeling::Body* m_body;

	}; // BodyPropertySetting
}

#endif // EMODELING_BODY_PROPERTY_SETTING_H
