#include "Scale9Symbol.h"

namespace glue
{

// Scale9Symbol::Scale9Symbol()
// {
// }

void Scale9Symbol::LoadJson(const Json::Value& val, const std::string& dir)
{
	const Json::Value& spr_val = val["sprite"];

	int w = val["width"].asInt(),
		h = val["height"].asInt();
	
	int index = 0;

	int idx0 = 0;
	s2::Sprite* grids[9];
	memset(grids, 0, sizeof(grids));
	s2::SCALE9_TYPE type = s2::SCALE9_TYPE(val["type"].asInt());
	switch (type)
	{
	case s2::S9_9GRID:
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(spr_val[idx0], dir);
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(spr_val[1], dir);
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(spr_val[2], dir);
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[3], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[4], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[5], dir);
		grids[s2::S9_TOP_LEFT]		= LoadSprite(spr_val[6], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[7], dir);
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(spr_val[8], dir);
		break;
	case s2::S9_9GRID_HOLLOW:
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(spr_val[idx0], dir);
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(spr_val[1], dir);
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(spr_val[2], dir);
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[3], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[4], dir);
		grids[s2::S9_TOP_LEFT]		= LoadSprite(spr_val[5], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[6], dir);
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(spr_val[7], dir);
		break;
	case s2::S9_3GRID_HORI:
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[idx0], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[1], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[2], dir);
		break;
	case s2::S9_3GRID_VERT:
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(spr_val[idx0], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[1], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[2], dir);
		break;
	case s2::S9_6GRID_UPPER:
		grids[s2::S9_MID_LEFT]		= LoadSprite(spr_val[idx0], dir);
		grids[s2::S9_MID_CENTER]	= LoadSprite(spr_val[1], dir);
		grids[s2::S9_MID_RIGHT]		= LoadSprite(spr_val[2], dir);
		grids[s2::S9_TOP_LEFT]		= LoadSprite(spr_val[3], dir);
		grids[s2::S9_TOP_CENTER]	= LoadSprite(spr_val[4], dir);
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(spr_val[5], dir);
		break;
	}

	m_s9.Build(type, w, h, grids);
}

s2::Sprite* Scale9Symbol::LoadSprite(const Json::Value& spr_val, const std::string& dir)
{
	std::string filepath = spr_val["filepath"].asString();
	filepath = glue::FilepathHelper::Absolute(dir, filepath);
	
	s2::Symbol* sym = glue::SymbolFactory::Instance()->Create(filepath);
	
}

}