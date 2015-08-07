#ifndef _LR_SELECT_SPRITES_OP_H_
#define _LR_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace etexture { class Sprite; }

namespace lr
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::ViewPanelMgr* view_panel_mgr = NULL, d2d::AbstractEditCMPT* callback = NULL);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

private:
	void UpdateShapeFromETexture(etexture::Sprite* spr);

private:
	d2d::Vector m_first_press;

}; // SelectSpritesOP

}

#endif // _LR_SELECT_SPRITES_OP_H_