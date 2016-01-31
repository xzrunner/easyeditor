#ifndef _ESHADER_SELECT_SPRITES_OP_H_
#define _ESHADER_SELECT_SPRITES_OP_H_



namespace eshader
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* spritesImpl, ee::EditCMPT* callback = NULL);

	virtual bool OnMouseLeftDown(int x, int y);

}; // SelectSpritesOP

}

#endif // _ESHADER_SELECT_SPRITES_OP_H_