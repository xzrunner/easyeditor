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
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual bool OnDraw() const;

	void SetGuides(ee::CrossGuides* guides) { m_guides = guides; }

private:
	void GroupSelection();
	void BreakUpSelection();

private:
	OpenSymbolDialog m_open_symbol;

	ee::CrossGuides* m_guides;

}; // SelectSpritesOP

}

#endif // _EASY_EASYCOMPLEX_SELECT_SPRITES_OP_H_