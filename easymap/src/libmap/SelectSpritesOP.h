#ifndef _EASYMAP_SELECT_SPRITES_OP_H_
#define _EASYMAP_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace emap
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::ViewPanelMgr* view_panel_mgr = NULL, d2d::AbstractEditCMPT* callback = NULL);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // SelectSpritesOP

}

#endif // _EASYMAP_SELECT_SPRITES_OP_H_