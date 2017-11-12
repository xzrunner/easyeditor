#ifndef _EASYDB_SELECT_SPRITES_OP_H_
#define _EASYDB_SELECT_SPRITES_OP_H_

#include <ee/SelectSpritesOP.h>

namespace edb
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode) override; 
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftDClick(int x, int y) override;

}; // SelectSpritesOP

}

#endif // _EASYDB_SELECT_SPRITES_OP_H_