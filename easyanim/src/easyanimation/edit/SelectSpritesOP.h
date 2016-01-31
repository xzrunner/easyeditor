#ifndef _EASYANIM_SELECT_SPRITES_OP_H_
#define _EASYANIM_SELECT_SPRITES_OP_H_



namespace eanim
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* spritesImpl, 
		ee::EditCMPT* callback = NULL);

	virtual bool OnMouseLeftDClick(int x, int y);

protected:
	virtual void PasteSprToClipboard(const ee::Sprite* spr, Json::Value& value) const;
	virtual void CopySprFromClipboard(ee::Sprite* spr, const Json::Value& value) const;

}; // SelectSpritesOP

}

#endif // _EASYANIM_SELECT_SPRITES_OP_H_