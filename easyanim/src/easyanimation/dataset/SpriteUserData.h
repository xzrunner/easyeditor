#ifndef _EASYANIM_SPRITE_USER_DATA_H_
#define _EASYANIM_SPRITE_USER_DATA_H_

#include <drag2d.h>

namespace eanim
{

struct SpriteUserData
{
	int layer;
	int frame;
}; // SpriteUserData

void set_sprite_user_data(d2d::ISprite* spr, int layer, int frame);

}

#endif // _EASYANIM_SPRITE_USER_DATA_H_