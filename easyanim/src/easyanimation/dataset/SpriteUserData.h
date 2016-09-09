#ifndef _EASYANIM_SPRITE_USER_DATA_H_
#define _EASYANIM_SPRITE_USER_DATA_H_

namespace ee { class Sprite; }

#include <fstream>

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

void set_sprite_user_data(ee::Sprite* spr, Layer* layer, KeyFrame* frame);
void set_sprite_user_data(ee::Sprite* spr, Layer* layer, KeyFrame* frame, std::ofstream& fout);
void set_sprite_user_data(ee::Sprite* spr, int layer_idx, int frame_idx);

}

#endif // _EASYANIM_SPRITE_USER_DATA_H_