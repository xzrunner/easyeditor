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

void SpriteUserData::SetSprData(ee::Sprite* spr, Layer* layer, KeyFrame* frame)
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

void SpriteUserData::SetSprData(ee::Sprite* spr, Layer* layer, KeyFrame* frame, std::ofstream& fout)
{
	printf("SpriteUserData::SetSprData 0, spr %p\n", spr);
	if (SpriteUserData* old_ud = (SpriteUserData*)spr->GetUserData())
	{
		printf("SpriteUserData::SetSprData 1, old_ud %p\n", old_ud);
		old_ud->layer = layer;
		old_ud->frame = frame;
		fout << "SpriteUserData::SetSprData 2\n";
	}
	else
	{
		fout << "SpriteUserData::SetSprData 3\n";
		SpriteUserData* ud = new SpriteUserData;
		fout << "SpriteUserData::SetSprData 4\n";
		ud->layer = layer;
		ud->frame = frame;
		fout << "SpriteUserData::SetSprData 5\n";
		spr->SetUserData(ud);
		fout << "SpriteUserData::SetSprData 6\n";
	}
	fout << "SpriteUserData::SetSprData 7\n";
}

void SpriteUserData::SetSprData(ee::Sprite* spr, int layer_idx, int frame_idx) 
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