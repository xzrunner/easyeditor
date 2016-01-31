#pragma once



namespace coceditor
{
	class SelectSpritesOP : public ee::SelectSpritesOP
	{
	public:
		SelectSpritesOP(ee::EditPanel* editPanel, ee::MultiSpritesImpl* spritesImpl, 
			ee::PropertySettingPanel* propertyPanel, ee::AbstractEditCMPT* callback = NULL,
			const ee::Colorf& color = ee::Colorf(0, 0, 0));

		virtual bool onMouseLeftDClick(int x, int y);

	}; // SelectSpritesOP
}

