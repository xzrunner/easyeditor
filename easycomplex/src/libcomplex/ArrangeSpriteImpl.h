#ifndef _EASY_EASYCOMPLEX_ARRANGE_SPRITE_IMPL_H_
#define _EASY_EASYCOMPLEX_ARRANGE_SPRITE_IMPL_H_

#include <ee/ArrangeSpriteImpl.h>

namespace ecomplex
{

class StagePanel;

class ArrangeSpriteImpl : public ee::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property);

protected:
	virtual ee::ArrangeSpriteState* CreateRotateState(ee::SpriteSelection* selection, 
		const ee::Vector& first_pos) const;

}; // ArrangeSpriteImpl

}

#endif // _EASY_EASYCOMPLEX_ARRANGE_SPRITE_IMPL_H_