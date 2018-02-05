#include "s2loader/AnimSymLoader.h"
#include "s2loader/EasyAnimLoader.h"
#include "s2loader/SpineAnimLoader.h"
#include "s2loader/ExtendSymFile.h"
#include "s2loader/BodymovinAnimLoader.h"

#include <sprite2/AnimSymbol.h>
#include <s2s/NodeFactory.h>
#include <s2s/AnimSym.h>
#include <memmgr/LinearAllocator.h>
#include <gum/FilepathHelper.h>

#include <json/json.h>

#include <fstream>

#include <string.h>

namespace s2loader
{

AnimSymLoader::AnimSymLoader(s2::AnimSymbol& sym,
							 const std::shared_ptr<const SymbolLoader>& sym_loader,
							 const std::shared_ptr<const SpriteLoader>& spr_loader)
	: m_sym(sym)
	, m_spr_loader(spr_loader)
	, m_sym_loader(sym_loader)
{
}

void AnimSymLoader::LoadJson(const CU_STR& filepath)
{
	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	int type = ExtendSymFile::GetType(val);
	switch (type)
	{
	case SYM_SPINE:
		{
			SpineAnimLoader loader(m_sym, m_sym_loader, m_spr_loader);
			loader.LoadJson(val, dir, filepath);
		}
		break;
	case SYM_BODYMOVIN:
		{
			BodymovinAnimLoader loader(m_sym, m_sym_loader, m_spr_loader);
			loader.LoadJson(val, dir);
		}
		break;
	case SYM_UNKNOWN:
		{
			EasyAnimLoader loader(m_sym, m_spr_loader);
			loader.LoadJson(val, dir);
		}
		break;
	}

	m_sym.LoadCopy();
	m_sym.BuildCurr();
}

void AnimSymLoader::LoadBin(const simp::NodeAnimation* node)
{
	EasyAnimLoader loader(m_sym, m_spr_loader);
	loader.LoadBin(node);

	m_sym.LoadCopy();
	m_sym.BuildCurr();
}

void AnimSymLoader::LoadSns(const CU_STR& filepath)
{
	mm::LinearAllocator alloc;
	auto sym = s2s::NodeFactory::CreateSymFromBin(alloc, filepath.c_str());
	assert(sym);

	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	EasyAnimLoader loader(m_sym, m_spr_loader);
	loader.LoadSns(dynamic_cast<s2s::AnimSym*>(sym), dir);

	m_sym.LoadCopy();
	m_sym.BuildCurr();
}

}