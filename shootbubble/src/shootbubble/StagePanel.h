#ifndef SHOOTBUBBLE_STAGE_PANEL_H_
#define SHOOTBUBBLE_STAGE_PANEL_H_

#include <drag2d.h>

namespace shootbubble
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

		void transCoordsToGridPos(const d2d::Vector& pos, int& row, int& col) const;
		void transGridPosToCoords(int row, int col, d2d::Vector& pos) const;

		void computeParams(float& hEdge, float& posOffset, float& posOffset1, 
			float& rowOffset) const;

		void updateAllSpritesLocation();

	private:
		d2d::Vector fixSpriteLocation(const d2d::Vector& pos) const;

	}; // StagePanel
}

#endif // SHOOTBUBBLE_STAGE_PANEL_H_