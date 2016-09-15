#include "Scale9SymLoader.h"
#include "FilepathHelper.h"
#include "SpriteFactory.h"
#include "SymbolFactory.h"

#include <sprite2/Scale9Symbol.h>
#include <sprite2/S2_Sprite.h>
#include <simp/NodeScale9.h>

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

void Scale9SymLoader::LoadBin(const simp::NodeScale9* node)
{
	s2::Sprite* grids[9];
	memset(grids, 0, sizeof(grids));

	const simp::NodeScale9::Grid* grid = NULL;

	int idx = 0;
	s2::SCALE9_TYPE type = s2::SCALE9_TYPE(node->type);
	switch (type)
	{
	case s2::S9_9GRID:
		grid = &node->grids[idx++];
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_RIGHT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		break;
	case s2::S9_9GRID_HOLLOW:
		grid = &node->grids[idx++];
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_RIGHT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		break;
	case s2::S9_3GRID_HORI:
		grid = &node->grids[idx++];
		grids[s2::S9_MID_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_RIGHT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		break;
	case s2::S9_3GRID_VERT:
		grid = &node->grids[idx++];
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		break;
	case s2::S9_6GRID_UPPER:
		grid = &node->grids[idx++];
		grids[s2::S9_MID_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_MID_RIGHT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_LEFT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_CENTER]	= LoadSprite(grid->sym, grid->dir, grid->mirror);
		grid = &node->grids[idx++];
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(grid->sym, grid->dir, grid->mirror);
		break;
	}

	const int DEFAULT_SIZE = 256;
	m_sym->GetScale9().Build(type, DEFAULT_SIZE, DEFAULT_SIZE, grids);

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

s2::Sprite* Scale9SymLoader::LoadSprite(uint32_t sym_id, uint16_t dir, uint16_t mirror)
{
	SymFileType type;
	s2::Symbol* sym = SymbolFactory::Instance()->Create(sym_id, &type);
	s2::Sprite* spr = SpriteFactory::Instance()->Create(sym, type);
	float angle = dir * SM_PI / 2;
	spr->SetAngle(angle);
	bool xmirror = mirror & 0x1,
		 ymirror = mirror & 0x2;
	sm::vec2 scale = spr->GetScale();
	if (xmirror) {
		scale.x = -fabs(scale.x);
	}
	if (ymirror) {
		scale.y = -fabs(scale.y);
	}
	spr->SetScale(scale);
	return spr;
}

}