#ifndef FORMATION_ACTOR_PROPERTY_SETTING_H
#define FORMATION_ACTOR_PROPERTY_SETTING_H

#include <drag2d.h>

namespace formation
{
	class ActorPropertySetting : public d2d::IPropertySetting
	{
	public:
		ActorPropertySetting(d2d::EditPanel* editPanel, 
			const std::vector<d2d::ISprite*>& sprites);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		int getID() const;

	private:
		std::vector<d2d::ISprite*> m_sprites;;

	}; // ActorPropertySetting
}

#endif // FORMATION_ACTOR_PROPERTY_SETTING_H