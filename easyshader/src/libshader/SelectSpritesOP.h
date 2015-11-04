#ifndef _ESHADER_SELECT_SPRITES_OP_H_
#define _ESHADER_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace eshader
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiSpritesImpl* spritesImpl, d2d::AbstractEditCMPT* callback = NULL);

	virtual bool OnMouseLeftDown(int x, int y);

}; // SelectSpritesOP

}

#endif // _ESHADER_SELECT_SPRITES_OP_H_