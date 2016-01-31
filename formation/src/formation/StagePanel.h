#ifndef FORMATION_STAGE_PANEL_H
#define FORMATION_STAGE_PANEL_H



namespace formation
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

		void resetAllSprites(float sx, float sy);

	}; // StagePanel
}

#endif // FORMATION_STAGE_PANEL_H