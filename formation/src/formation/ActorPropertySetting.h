#ifndef FORMATION_ACTOR_PROPERTY_SETTING_H
#define FORMATION_ACTOR_PROPERTY_SETTING_H



namespace formation
{
	class ActorPropertySetting : public ee::IPropertySetting
	{
	public:
		ActorPropertySetting(ee::EditPanel* editPanel, 
			const std::vector<ee::ISprite*>& sprites);

		virtual void updatePanel(ee::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(ee::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(ee::PropertySettingPanel* panel, bool bEnable);

	private:
		int getID() const;

	private:
		std::vector<ee::ISprite*> m_sprites;;

	}; // ActorPropertySetting
}

#endif // FORMATION_ACTOR_PROPERTY_SETTING_H