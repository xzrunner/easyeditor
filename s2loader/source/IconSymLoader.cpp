#include "s2loader/IconSymLoader.h"
#include "s2loader/IconType.h"
#include "s2loader/ArrayLoader.h"

#include <sprite2/IconSymbol.h>
#include <sprite2/StaticRectIcon.h>
#include <sprite2/StaticQuadIcon.h>
#include <sprite2/DynamicRectIcon.h>
#include <sprite2/DynamicSectorIcon.h>
#include <sprite2/ImageSymbol.h>
#include <simp/NodeIcon.h>
#include <memmgr/LinearAllocator.h>
#include <s2s/IconSym.h>
#include <s2s/NodeFactory.h>
#include <gum/FilepathHelper.h>
#include <gum/SymbolPool.h>

#include <sprite2/SymType.h>

#include <fstream>

#include <assert.h>

namespace s2loader
{

IconSymLoader::IconSymLoader(s2::IconSymbol& sym)
	: m_sym(sym)
{
}

void IconSymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	// todo
}

void IconSymLoader::LoadBin(const simp::NodeIcon* node)
{
	CU_VEC<sm::vec2> vertices;
	ArrayLoader::Load(vertices, node->vertices, node->vertices_n, 16);

	std::unique_ptr<s2::Icon> icon = CreateIcon(node->type, vertices);

	auto base = gum::SymbolPool::Instance()->Fetch(node->base_id);
	if (base) {
		assert(base->Type() == s2::SYM_IMAGE);
		icon->SetImage(S2_VI_PTR_DOWN_CAST<s2::ImageSymbol>(base));
	}

	if (icon) {
		m_sym.SetIcon(icon);
	}
}

void IconSymLoader::LoadSns(const CU_STR& filepath)
{
	mm::LinearAllocator alloc;
	auto sym = s2s::NodeFactory::CreateSymFromBin(alloc, filepath.c_str());
	assert(sym);
	auto icon_sym = dynamic_cast<s2s::IconSym*>(sym);
	assert(icon_sym);

	CU_VEC<sm::vec2> vertices;
	auto* src_vertices = icon_sym->GetVertices();
	int src_vertices_n = icon_sym->GetVerticesNum();
	vertices.reserve(src_vertices_n / 2);
	int idx = 0;
	for (int i = 0, n = src_vertices_n / 2; i< n; ++i) {
		float x = static_cast<float>(src_vertices[idx++]) / s2s::IconSym::FIXED_TRANS_PRECISION;
		float y = static_cast<float>(src_vertices[idx++]) / s2s::IconSym::FIXED_TRANS_PRECISION;
		vertices.push_back(sm::vec2(x, y));
	}
	
	std::unique_ptr<s2::Icon> icon = CreateIcon(icon_sym->GetType(), vertices);

	CU_STR dir = gum::FilepathHelper::Dir(filepath);
	CU_STR base_sym_path = gum::FilepathHelper::Absolute(dir, icon_sym->GetBaseSymPath());	
	auto base = gum::SymbolPool::Instance()->Fetch(base_sym_path);
	if (base) {
		assert(base->Type() == s2::SYM_IMAGE);
		icon->SetImage(S2_VI_PTR_DOWN_CAST<s2::ImageSymbol>(base));
	}

	if (icon) {
		m_sym.SetIcon(icon);
	}
}

std::unique_ptr<s2::Icon> IconSymLoader::CreateIcon(int type, const CU_VEC<sm::vec2>& vertices)
{
	std::unique_ptr<s2::Icon> icon;
	switch (type)
	{
	case s2loader::ICON_RECT:
		{
			auto rect = std::make_unique<s2::StaticRectIcon>();
			rect->SetRegion(sm::rect(vertices[0], vertices[1]));
			icon = std::move(rect);
		}
		break;
	case s2loader::ICON_QUAD:
		{
			auto quad = std::make_unique<s2::StaticQuadIcon>();
			quad->SetScreen(&vertices[0]);
			icon = std::move(quad);
		}
		break;
	case s2loader::ICON_CHANGED_RECT:
		{
			auto drect = std::make_unique<s2::DynamicRectIcon>();
			drect->SetRegion(
				sm::rect(vertices[0], vertices[1]),
				sm::rect(vertices[2], vertices[3]));
			icon = std::move(drect);
		}
		break;
	case s2loader::ICON_CHANGED_SECTOR:
		{
			auto sector = std::make_unique<s2::DynamicSectorIcon>();
			sector->SetRegion(vertices[0].x, vertices[0].y);
			icon = std::move(sector);
		}
		break;
	}
	return std::move(icon);
}

}