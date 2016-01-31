#ifndef _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_
#define _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_



namespace etexpacker
{
	class StagePanel;

	class ArrangeSpriteOP : public ee::ArrangeSpriteOP<ee::SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* editPanel, 
			ee::PropertySettingPanel* propertyPanel,
			ee::EditCMPT* callback = NULL);
	}; // ArrangeSpriteOP
}

#endif // _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_