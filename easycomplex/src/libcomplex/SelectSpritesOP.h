#ifndef _EASYCOMPLEX_SELECT_SPRITES_OP_H_
#define _EASYCOMPLEX_SELECT_SPRITES_OP_H_

#include "OpenSymbolDialog.h"

#include <ee/SelectSpritesOP.h>

namespace ecomplex
{

class SelectSpritesOP : public ee::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseLeftDClick(int x, int y) override;

	virtual bool OnDraw() const override;

	void SetGuides(ee::CrossGuides* guides) { m_guides = guides; }

	// todo
	void SetOpenSymbolDialogViewlist(ee::ViewlistPanel* viewlist) {
		m_open_symbol.SetViewlist(viewlist);
	}

private:
	OpenSymbolDialog m_open_symbol;

	ee::CrossGuides* m_guides;

}; // SelectSpritesOP

}

#endif // _EASYCOMPLEX_SELECT_SPRITES_OP_H_