#include "s2loader/Scale9SymLoader.h"
#include "s2loader/SpriteFactory.h"
#include "s2loader/SpriteLoader.h"
#include "s2loader/SprTransLoader.h"

#include <sprite2/Scale9Symbol.h>
#include <sprite2/Sprite.h>
#include <simp/NodeScale9.h>
#include <sm_const.h>
#include <memmgr/LinearAllocator.h>
#include <s2s/NodeFactory.h>
#include <s2s/NodeSprCommon.h>
#include <s2s/Scale9Sym.h>
#include <s2s/NodeSpr.h>
#include <gum/FilepathHelper.h>
#include <gum/SymbolPool.h>

#include <fstream>

#include <string.h>

namespace s2loader
{

Scale9SymLoader::Scale9SymLoader(s2::Scale9Symbol& sym, 
	                             const std::shared_ptr<const SpriteLoader>& spr_loader)
	: m_sym(sym)
	, m_spr_loader(spr_loader)
{
	if (!m_spr_loader) {
		m_spr_loader = std::make_shared<SpriteLoader>();
	}
}

void Scale9SymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	const Json::Value& spr_val = val["sprite"];

	s2::SprPtr grids[9];
	memset(grids, 0, sizeof(grids));

	int idx = 0;
	s2::SCALE9_TYPE type = s2::SCALE9_TYPE(val["type"].asInt());
	switch (type)
	{
	case s2::S9_9GRID:
		grids[s2::S9_DOWN_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_DOWN_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_DOWN_RIGHT]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_RIGHT]		= m_spr_loader->Create(spr_val[idx++], dir);
		break;
	case s2::S9_9GRID_HOLLOW:
		grids[s2::S9_DOWN_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_DOWN_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_DOWN_RIGHT]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_RIGHT]		= m_spr_loader->Create(spr_val[idx++], dir);
		break;
	case s2::S9_3GRID_HORI:
		grids[s2::S9_MID_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= m_spr_loader->Create(spr_val[idx++], dir);
		break;
	case s2::S9_3GRID_VERT:
		grids[s2::S9_DOWN_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		break;
	case s2::S9_6GRID_UPPER:
		grids[s2::S9_MID_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_MID_RIGHT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_LEFT]		= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_CENTER]	= m_spr_loader->Create(spr_val[idx++], dir);
		grids[s2::S9_TOP_RIGHT]		= m_spr_loader->Create(spr_val[idx++], dir);
		break;
	default:
		break;
	}

	int w = val["width"].asInt(),
		h = val["height"].asInt();
	m_sym.GetScale9().Build(type, w, h, grids, 0, 0, 0, 0);
}

void Scale9SymLoader::LoadBin(const simp::NodeScale9* node)
{
	s2::SprPtr grids[9];
	memset(grids, 0, sizeof(grids));

	const simp::NodeScale9::Grid* grid = nullptr;

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
	default:
		break;
	}

	const int DEFAULT_SIZE = 256;
	m_sym.GetScale9().Build(type, DEFAULT_SIZE, DEFAULT_SIZE, grids, 
		node->left, node->right, node->top, node->down);
}

void Scale9SymLoader::LoadSns(const CU_STR& filepath)
{
	mm::LinearAllocator alloc;
	auto sym = s2s::NodeFactory::CreateSymFromBin(alloc, filepath.c_str());
	assert(sym);
	auto s9_sym = dynamic_cast<s2s::Scale9Sym*>(sym);
	assert(s9_sym);

	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	s2::SprPtr grids[9];
	memset(grids, 0, sizeof(grids));

	int idx = 0;
	s2::SCALE9_TYPE type = s2::SCALE9_TYPE(s9_sym->Type());
	switch (type)
	{
	case s2::S9_9GRID:
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_DOWN_LEFT)->GetCommon());
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_DOWN_CENTER)->GetCommon());
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_DOWN_RIGHT)->GetCommon());
		grids[s2::S9_MID_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_LEFT)->GetCommon());
		grids[s2::S9_MID_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_CENTER)->GetCommon());
		grids[s2::S9_MID_RIGHT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_RIGHT)->GetCommon());
		grids[s2::S9_TOP_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_LEFT)->GetCommon());
		grids[s2::S9_TOP_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_CENTER)->GetCommon());
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_RIGHT)->GetCommon());
		break;
	case s2::S9_9GRID_HOLLOW:
		grids[s2::S9_DOWN_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_DOWN_LEFT)->GetCommon());
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_DOWN_CENTER)->GetCommon());
		grids[s2::S9_DOWN_RIGHT]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_DOWN_RIGHT)->GetCommon());
		grids[s2::S9_MID_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_LEFT)->GetCommon());
		grids[s2::S9_MID_RIGHT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_RIGHT)->GetCommon());
		grids[s2::S9_TOP_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_LEFT)->GetCommon());
		grids[s2::S9_TOP_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_CENTER)->GetCommon());
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_RIGHT)->GetCommon());
		break;
	case s2::S9_3GRID_HORI:
		grids[s2::S9_MID_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_LEFT)->GetCommon());
		grids[s2::S9_MID_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_CENTER)->GetCommon());
		grids[s2::S9_MID_RIGHT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_RIGHT)->GetCommon());
		break;
	case s2::S9_3GRID_VERT:
		grids[s2::S9_DOWN_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_DOWN_CENTER)->GetCommon());
		grids[s2::S9_MID_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_CENTER)->GetCommon());
		grids[s2::S9_TOP_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_CENTER)->GetCommon());
		break;
	case s2::S9_6GRID_UPPER:
		grids[s2::S9_MID_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_LEFT)->GetCommon());
		grids[s2::S9_MID_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_CENTER)->GetCommon());
		grids[s2::S9_MID_RIGHT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_MID_RIGHT)->GetCommon());
		grids[s2::S9_TOP_LEFT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_LEFT)->GetCommon());
		grids[s2::S9_TOP_CENTER]	= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_CENTER)->GetCommon());
		grids[s2::S9_TOP_RIGHT]		= LoadSprite(s9_sym->GetChildByIndex(s2s::Scale9Sym::S9_TOP_RIGHT)->GetCommon());
		break;
	default:
		break;
	}

	const int DEFAULT_SIZE = 256;
	int left, right, top, down;
	s9_sym->GetSize(left, right, top, down);
	m_sym.GetScale9().Build(type, DEFAULT_SIZE, DEFAULT_SIZE, grids, 
		left, right, top, down);
}

s2::SprPtr Scale9SymLoader::LoadSprite(uint32_t sym_id, uint16_t dir, uint16_t mirror)
{
	auto sym = gum::SymbolPool::Instance()->Fetch(sym_id);
	if (!sym) {
		return nullptr;
	}
	auto spr = SpriteFactory::Instance()->Create(sym);
	if (!spr) {
		return nullptr;
	}
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

s2::SprPtr Scale9SymLoader::LoadSprite(const s2s::NodeSprCommon& spr_common)
{
	auto spr = SpriteFactory::Instance()->Create(spr_common.GetFilepath());
	if (spr) {
		SprTransLoader::Load(spr, spr_common);
	}
	return spr;
}

}