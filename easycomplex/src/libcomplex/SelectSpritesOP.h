#ifndef _EASY_EASYCOMPLEX_SELECT_SPRITES_OP_H_
#define _EASY_EASYCOMPLEX_SELECT_SPRITES_OP_H_

#include "OpenSymbolDialog.h"

#include <ee/SelectSpritesOP.h>

namespace ecomplex
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDClick(int x, int y);

private:
	void GroupSelection();
	void BreakUpSelection();

private:
	OpenSymbolDialog m_open_symbol;

}; // SelectSpritesOP

}

#endif // _EASY_EASYCOMPLEX_SELECT_SPRITES_OP_H_