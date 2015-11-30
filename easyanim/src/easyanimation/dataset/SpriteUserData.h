#ifndef _EASYANIM_SPRITE_USER_DATA_H_
#define _EASYANIM_SPRITE_USER_DATA_H_

#include <drag2d.h>

namespace eanim
{

class LayersMgr;
class Layer;
class KeyFrame;

struct SpriteUserData
{
	LayersMgr* all_layers;
	Layer* layer;
	KeyFrame* frame;

	int layer_idx;
	int frame_idx;

	SpriteUserData();

}; // SpriteUserData

void set_sprite_user_data(d2d::ISprite* spr, LayersMgr* all_layers, 
						  Layer* layer, KeyFrame* frame);
void set_sprite_user_data(d2d::ISprite* spr, int layer_idx, int frame_idx);

}

#endif // _EASYANIM_SPRITE_USER_DATA_H_