#ifndef _EASYCOMPLEX_ARRANGE_SPRITE_IMPL_H_
#define _EASYCOMPLEX_ARRANGE_SPRITE_IMPL_H_

#include <drag2d.h>

namespace ecomplex
{

class StagePanel;

class ArrangeSpriteImpl : public d2d::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, d2d::PropertySettingPanel* property);

protected:
	virtual d2d::IArrangeSpriteState* CreateRotateState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;

}; // ArrangeSpriteImpl

}

#endif // _EASYCOMPLEX_ARRANGE_SPRITE_IMPL_H_