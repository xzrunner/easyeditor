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

void SpriteUserData::SetSprData(ee::Sprite& spr, Layer* layer, KeyFrame* frame)
{
	if (SpriteUserData* old_ud = (SpriteUserData*)spr.GetUserData())
	{
		old_ud->layer = layer;
		old_ud->frame = frame;
	}
	else
	{
		SpriteUserData* ud = new SpriteUserData;
		ud->layer = layer;
		ud->frame = frame;
		spr.SetUserData(ud);
	}
}

void SpriteUserData::SetSprData(ee::Sprite& spr, int layer_idx, int frame_idx) 
{
	if (SpriteUserData* old_ud = (SpriteUserData*)spr.GetUserData())
	{
		old_ud->layer_idx = layer_idx;
		old_ud->frame_idx = frame_idx;
	}
	else
	{
		SpriteUserData* ud = new SpriteUserData;
		ud->layer_idx = layer_idx;
		ud->frame_idx = frame_idx;
		spr.SetUserData(ud);
	}
}

}