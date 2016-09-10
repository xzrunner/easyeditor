#include "Scale9SymLoader.h"
#include "FilepathHelper.h"
#include "SpriteFactory.h"

#include <sprite2/Scale9Symbol.h>
#include <sprite2/S2_Sprite.h>

#include <fstream>

namespace gum
{

Scale9SymLoader::Scale9SymLoader(s2::Scale9Symbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

Scale9SymLoader::~Scale9SymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void Scale9SymLoader::LoadJson(const std::string& filepath)
{
	if (!m_sym) {
		return;
	}

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
	m_sym->GetScale9().Build(type, w, h, grids);

	for (int i = 0; i < 9; ++i) {
		if (grids[i]) {
			grids[i]->RemoveReference();
		}
	}
}

s2::Sprite* Scale9SymLoader::LoadSprite(const Json::Value& val, const std::string& dir)
{
	return SpriteFactory::Instance()->Create(val, dir);
}

}