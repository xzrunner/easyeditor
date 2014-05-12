#ifndef FORMATION_STAGE_PANEL_H
#define FORMATION_STAGE_PANEL_H

#include <drag2d.h>

namespace formation
{
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
		virtual void insertSprite(d2d::ISprite* sprite);

		void resetAllSprites(float sx, float sy);

	}; // StagePanel
}

#endif // FORMATION_STAGE_PANEL_H