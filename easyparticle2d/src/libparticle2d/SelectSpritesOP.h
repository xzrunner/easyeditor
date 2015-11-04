#ifndef _EASYPARTICLE2D_SELECT_SPRITES_OP_H_
#define _EASYPARTICLE2D_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace eparticle2d
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::AbstractEditCMPT* callback = NULL);

//	virtual bool OnMouseLeftDown(int x, int y);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);

private:
	void OnSelected();

}; // SelectSpritesOP

}

#endif // _EASYPARTICLE2D_SELECT_SPRITES_OP_H_