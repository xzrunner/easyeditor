#ifndef _LR_SELECT_SPRITES_OP_H_
#define _LR_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace lr
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* propertyPanel = NULL, d2d::AbstractEditCMPT* callback = NULL);

	virtual bool onMouseLeftDClick(int x, int y);

}; // SelectSpritesOP

}

#endif // _LR_SELECT_SPRITES_OP_H_