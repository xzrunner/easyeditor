#include "Scale9Symbol.h"
#include "SpriteFactory.h"
#include "FilepathHelper.h"

#include <sprite2/S2_Sprite.h>

#include <fstream>

namespace glue
{

// Scale9Symbol::Scale9Symbol()
// {
// }

void Scale9Symbol::LoadJson(const std::string& filepath)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = FilepathHelper::Dir(filepath);

	const Json::Value& spr_val = val["sprite"];
	
	s2::Sprite* grids[9];
	memset(grids, 0, sizeof(grids));

	int idx = 0;
	s2::SCALE9_TYPE type = s2::SCALE9_TYPE(val["type"].asInt());
	switch (type)
	{
	case s2::S9_9GRID:
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(spr_val[idx++], dir);
		break;
	case s2::S9_9GRID_HOLLOW:
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(spr_val[idx++], dir);
		break;
	case s2::S9_3GRID_HORI:
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[idx++], dir);
		break;
	case s2::S9_3GRID_VERT:
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[idx++], dir);
		break;
	case s2::S9_6GRID_UPPER:
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_LEFT]		= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[idx++], dir);
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(spr_val[idx++], dir);
		break;
	}

	int w = val["width"].asInt(),
		h = val["height"].asInt();
	m_s9.Build(type, w, h, grids);
}

s2::Sprite* Scale9Symbol::LoadSprite(const Json::Value& val, const std::string& dir)
{
	return SpriteFactory::Instance()->Create(val, dir);
}

}