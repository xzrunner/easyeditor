#ifndef RAIDEN_SELECT_SPRITES_OP_H
#define RAIDEN_SELECT_SPRITES_OP_H



namespace raiden
{
	class SelectSpritesOP : public ee::SelectSpritesOP
	{
	public:
		SelectSpritesOP(ee::EditPanel* editPanel, ee::MultiSpritesImpl* spritesImpl, 
			ee::PropertySettingPanel* propertyPanel, ee::AbstractEditCMPT* callback = NULL);

		virtual bool onMouseLeftDClick(int x, int y);

		virtual ee::IPropertySetting* createPropertySetting(ee::ISprite* sprite) const;
		virtual ee::IPropertySetting* createPropertySetting(const std::vector<ee::ISprite*>& sprites) const;

	}; // SelectSpritesOP
}

#endif // RAIDEN_SELECT_SPRITES_OP_H