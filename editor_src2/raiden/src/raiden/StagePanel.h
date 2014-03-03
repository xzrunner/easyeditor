#ifndef RAIDEN_STAGE_PANEL_H
#define RAIDEN_STAGE_PANEL_H

#include <drag2d.h>

namespace raiden
{
	class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::SpritesPanelImpl interface
		//
		virtual void traverseSprites(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting, bool order = true) const;
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

		void resetPathOffset(d2d::ISymbol* symbol);

	}; // StagePanel
}

#endif // RAIDEN_STAGE_PANEL_H