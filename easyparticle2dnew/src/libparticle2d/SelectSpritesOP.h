#ifndef _EASYPARTICLE2D_SELECT_SPRITES_OP_H_
#define _EASYPARTICLE2D_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace eparticle2d
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL);

	virtual bool onMouseLeftDown(int x, int y);

}; // SelectSpritesOP

}

#endif // _EASYPARTICLE2D_SELECT_SPRITES_OP_H_