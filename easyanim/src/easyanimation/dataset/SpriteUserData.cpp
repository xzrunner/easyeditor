#include "SpriteUserData.h"

#include <ee/Sprite.h>

#include <stddef.h>

namespace eanim
{

SpriteUserData::SpriteUserData()
	: layer(NULL)
	, frame(NULL)
{
	layer_idx = frame_idx = -1;
}

void set_sprite_user_data(ee::Sprite* spr, Layer* layer, KeyFrame* frame)
{
	if (SpriteUserData* old_ud = (SpriteUserData*)spr->GetUserData())
	{
		old_ud->layer = layer;
		old_ud->frame = frame;
	}
	else
	{
		SpriteUserData* ud = new SpriteUserData;
		ud->layer = layer;
		ud->frame = frame;
		spr->SetUserData(ud);
	}
}

void set_sprite_user_data(ee::Sprite* spr, Layer* layer, KeyFrame* frame, std::ofstream& fout)
{
	printf("set_sprite_user_data 0, spr %p\n", spr);
	if (SpriteUserData* old_ud = (SpriteUserData*)spr->GetUserData())
	{
		printf("set_sprite_user_data 1, old_ud %p\n", old_ud);
		old_ud->layer = layer;
		old_ud->frame = frame;
		fout << "set_sprite_user_data 2\n";
	}
	else
	{
		fout << "set_sprite_user_data 3\n";
		SpriteUserData* ud = new SpriteUserData;
		fout << "set_sprite_user_data 4\n";
		ud->layer = layer;
		ud->frame = frame;
		fout << "set_sprite_user_data 5\n";
		spr->SetUserData(ud);
		fout << "set_sprite_user_data 6\n";
	}
	fout << "set_sprite_user_data 7\n";
}

void set_sprite_user_data(ee::Sprite* spr, int layer_idx, int frame_idx) 
{
	if (SpriteUserData* old_ud = (SpriteUserData*)spr->GetUserData())
	{
		old_ud->layer_idx = layer_idx;
		old_ud->frame_idx = frame_idx;
	}
	else
	{
		SpriteUserData* ud = new SpriteUserData;
		ud->layer_idx = layer_idx;
		ud->frame_idx = frame_idx;
		spr->SetUserData(ud);
	}
}

}