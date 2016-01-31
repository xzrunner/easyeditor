#ifndef _EASYCOMPLEX_SELECT_SPRITES_OP_H_
#define _EASYCOMPLEX_SELECT_SPRITES_OP_H_



#include "OpenSymbolDialog.h"

namespace ecomplex
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* spritesImpl, ee::EditCMPT* callback = NULL);

//		virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDClick(int x, int y);

private:
	OpenSymbolDialog m_open_symbol;

}; // SelectSpritesOP

}

#endif // _EASYCOMPLEX_SELECT_SPRITES_OP_H_