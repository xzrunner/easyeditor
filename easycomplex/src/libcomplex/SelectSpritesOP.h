#ifndef _EASYCOMPLEX_SELECT_SPRITES_OP_H_
#define _EASYCOMPLEX_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace ecomplex
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::ViewPanelMgr* view_panel_mgr,d2d::AbstractEditCMPT* callback = NULL);

//		virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDClick(int x, int y);

}; // SelectSpritesOP

}

#endif // _EASYCOMPLEX_SELECT_SPRITES_OP_H_