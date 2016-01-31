#ifndef RAIDEN_STAGE_PANEL_H
#define RAIDEN_STAGE_PANEL_H



namespace raiden
{
	class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

		//
		// ee::EditPanel interface
		//
		virtual void clear();

		//
		// ee::SpritesPanelImpl interface
		//
		virtual void traverseSprites(ee::IVisitor& visitor, 
			ee::TraverseType type = ee::e_allExisting, bool order = true) const;
		virtual void removeSprite(ee::ISprite* sprite);
		virtual void insertSprite(ee::ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(ee::ISprite* sprite, bool up);

		void resetPathOffset(ee::ISymbol* symbol);

	}; // StagePanel
}

#endif // RAIDEN_STAGE_PANEL_H