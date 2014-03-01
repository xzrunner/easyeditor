#ifndef EPACKER_STAGE_PANEL_H
#define EPACKER_STAGE_PANEL_H

#include <drag2d.h>

namespace epacker
{
	class BaseStrategy;

	class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
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
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);

		void insertSpriteNoArrange(d2d::ISprite* sprite);

		void arrangeAllSprites(bool bClearSelection);
		void loadFromLibrary();

	public:
		static void fixCoords(d2d::ISprite* sprite);

	private:
		BaseStrategy* m_strategy;

	}; // StagePanel
}

#endif // EPACKER_STAGE_PANEL_H