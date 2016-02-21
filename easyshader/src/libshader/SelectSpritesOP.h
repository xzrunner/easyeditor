#ifndef _EASYSHADER_SELECT_SPRITES_OP_H_
#define _EASYSHADER_SELECT_SPRITES_OP_H_

#include <ee/SelectSpritesOP.h>

namespace eshader
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, ee::EditCMPT* callback = NULL);

	virtual bool OnMouseLeftDown(int x, int y);

}; // SelectSpritesOP

}

#endif // _EASYSHADER_SELECT_SPRITES_OP_H_