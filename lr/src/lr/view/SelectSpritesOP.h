#ifndef _LR_SELECT_SPRITES_OP_H_
#define _LR_SELECT_SPRITES_OP_H_

#include <drag2d.h>

#include "OpenSymbolDialog.h"

namespace lr
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::AbstractEditCMPT* callback = NULL);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

private:
	d2d::Vector m_first_press;

	OpenSymbolDialog m_open_symbol;

}; // SelectSpritesOP

}

#endif // _LR_SELECT_SPRITES_OP_H_