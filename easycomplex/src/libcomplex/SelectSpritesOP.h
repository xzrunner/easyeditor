#ifndef _EASYCOMPLEX_SELECT_SPRITES_OP_H_
#define _EASYCOMPLEX_SELECT_SPRITES_OP_H_

#include <drag2d.h>

#include "OpenSymbolDialog.h"

namespace ecomplex
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiSpritesImpl* spritesImpl, d2d::AbstractEditCMPT* callback = NULL);

//		virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDClick(int x, int y);

private:
	OpenSymbolDialog m_open_symbol;

}; // SelectSpritesOP

}

#endif // _EASYCOMPLEX_SELECT_SPRITES_OP_H_