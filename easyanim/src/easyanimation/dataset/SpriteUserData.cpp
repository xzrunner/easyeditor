#include "SpriteUserData.h"

namespace eanim
{

void set_sprite_user_data(d2d::ISprite* spr, int layer, int frame)
{
	if (SpriteUserData* old_ud = (SpriteUserData*)spr->getUserData())
	{
		old_ud->layer = layer;
		old_ud->frame = frame;
	}
	else
	{
		SpriteUserData* ud = new SpriteUserData;
		ud->layer = layer;
		ud->frame = frame;
		spr->setUserData(ud);
	}
}

}