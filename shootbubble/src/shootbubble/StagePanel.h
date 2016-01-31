#ifndef SHOOTBUBBLE_STAGE_PANEL_H_
#define SHOOTBUBBLE_STAGE_PANEL_H_



namespace shootbubble
{
	class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
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
		virtual void insertSprite(ee::ISprite* sprite);

		void transCoordsToGridPos(const ee::Vector& pos, int& row, int& col) const;
		void transGridPosToCoords(int row, int col, ee::Vector& pos) const;

		void computeParams(float& hEdge, float& posOffset, float& posOffset1, 
			float& rowOffset) const;

		void updateAllSpritesLocation();

	private:
		ee::Vector fixSpriteLocation(const ee::Vector& pos) const;

	}; // StagePanel
}

#endif // SHOOTBUBBLE_STAGE_PANEL_H_