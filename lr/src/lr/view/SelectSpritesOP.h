#ifndef _LR_SELECT_SPRITES_OP_H_
#define _LR_SELECT_SPRITES_OP_H_

#include "OpenSymbolDialog.h"

#include <ee/SelectSpritesOP.h>

namespace lr
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
		ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

private:
	void BuildGroup();
	void BreakUpGroup();

	void BuildComplex();
	void BreakUpComplex();

private:
	sm::vec2 m_first_press;

	OpenSymbolDialog m_open_symbol;

}; // SelectSpritesOP

}

#endif // _LR_SELECT_SPRITES_OP_H_