#ifndef _EASYANIM_SPRITE_USER_DATA_H_
#define _EASYANIM_SPRITE_USER_DATA_H_

#include <drag2d.h>

namespace eanim
{

class Layer;
class KeyFrame;

struct SpriteUserData
{
	Layer* layer;
	KeyFrame* frame;

	int layer_idx;
	int frame_idx;

	SpriteUserData();

}; // SpriteUserData

void set_sprite_user_data(d2d::Sprite* spr, Layer* layer, KeyFrame* frame);
void set_sprite_user_data(d2d::Sprite* spr, int layer_idx, int frame_idx);

}

#endif // _EASYANIM_SPRITE_USER_DATA_H_