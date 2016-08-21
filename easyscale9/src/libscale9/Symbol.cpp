#include "Symbol.h"
#include "config.h"

#include <ee/StringHelper.h>
#include <ee/FileHelper.h>
#include <ee/Sprite.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + ee::StringHelper::ToString(id++);
}

// void Symbol::ResizeScale9(float width, float height)
// {
// 	m_data.Resize(width, height);
// }

// void Symbol::ComposeFromSprites(ee::Sprite* sprs[3][3], float width, float height)
// {
// 	m_data.Compose(sprs, width, height);
// }

void Symbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Json::Value spr_val = value["sprite"];

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);

	int width = value["width"].asInt(),
		height = value["height"].asInt();

	int index = 0;

	int idx0 = 0;
	s2::Sprite* grids[9];
	memset(grids, 0, sizeof(grids));
	s2::SCALE9_TYPE type = s2::SCALE9_TYPE(value["type"].asInt());
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
	
	m_s9.Build(type, width, height, grids);
}

ee::Sprite* Symbol::LoadSprite(const Json::Value& spr_val, const std::string& dir)
{
	std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spr_val);
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ee::SymbolSearcher::SetSymbolFilepaths(dir, sym, spr_val);
	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Load(spr_val);
	sym->RemoveReference();
	return spr;
}

}