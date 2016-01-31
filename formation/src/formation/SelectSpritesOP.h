#ifndef FORMATION_SELECT_SPRITES_OP_H
#define FORMATION_SELECT_SPRITES_OP_H



namespace formation
{
	class SelectSpritesOP : public ee::SelectSpritesOP
	{
	public:
		SelectSpritesOP(ee::EditPanel* editPanel, ee::MultiSpritesImpl* spritesImpl, 
			ee::PropertySettingPanel* propertyPanel, ee::AbstractEditCMPT* callback = NULL);

		virtual ee::IPropertySetting* createPropertySetting(ee::ISprite* sprite) const;
		virtual ee::IPropertySetting* createPropertySetting(const std::vector<ee::ISprite*>& sprites) const;

	}; // SelectSpritesOP
}

#endif // FORMATION_SELECT_SPRITES_OP_H