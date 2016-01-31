#ifndef RAIDEN_ACTOR_PROPERTY_SETTING_H
#define RAIDEN_ACTOR_PROPERTY_SETTING_H



namespace raiden
{
	class ActorPropertySetting : public ee::MultiSpritesPropertySetting
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
		int getSpeed() const;

	}; // ActorPropertySetting
}

#endif // RAIDEN_ACTOR_PROPERTY_SETTING_H