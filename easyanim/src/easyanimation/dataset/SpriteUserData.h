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

	static void SetSprData(ee::Sprite* spr, Layer* layer, KeyFrame* frame);	
	static void SetSprData(ee::Sprite* spr, Layer* layer, KeyFrame* frame, std::ofstream& fout);
	static void SetSprData(ee::Sprite* spr, int layer_idx, int frame_idx);	

}; // SpriteUserData

}

#endif // _EASYANIM_SPRITE_USER_DATA_H_