#pragma once



namespace eanim
{
	class StagePanel;

	class ArrangeSpriteOP : public ee::ArrangeSpriteOP<ee::SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* editPanel);

		virtual bool onMouseLeftDClick(int x, int y);

	private:
		StagePanel* m_editPanel;

	}; // ArrangeSpriteOP
}

