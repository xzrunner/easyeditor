#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/Visitor.h>
#include <ee/SymbolLoader.h>
#include <ee/SpriteLoader.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>

#include <sprite2/SymType.h>
#include <sprite2/Sprite.h>
#include <s2loader/AnimSymLoader.h>
#include <gum/StringHelper.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace libanim
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + gum::StringHelper::ToString(id++).c_str();
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		const auto& layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j)
		{
			const auto& frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				auto sym = frame->sprs[k]->GetSymbol();
				std::dynamic_pointer_cast<ee::Symbol>(sym)->ReloadTexture();
			}
		}
	}
}

void Symbol::Traverse(ee::RefVisitor<ee::Sprite>& visitor)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		const auto& layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j)
		{
			const auto& frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				auto spr = std::dynamic_pointer_cast<ee::Sprite>(frame->sprs[k]);
				bool next;
				visitor.Visit(spr, next);
				std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->Traverse(visitor);
			}
		}
	}
}

void Symbol::Load(const std::shared_ptr<s2loader::SpriteLoader>& spr_loader)
{
	Clear();

	auto sym_loader(std::make_shared<ee::SymbolLoader>());
	s2loader::AnimSymLoader loader(*this, sym_loader, spr_loader);
	loader.LoadJson(m_filepath.c_str());

	LoadEE();
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	auto sym_loader(std::make_shared<ee::SymbolLoader>());
	auto spr_loader(std::make_shared<ee::SpriteLoader>());
	s2loader::AnimSymLoader loader(*this, sym_loader, spr_loader);
	
	auto ext = ee::FileHelper::GetExtension(m_filepath);
	if (ext == "json") {
		loader.LoadJson(m_filepath.c_str());
		LoadEE();
	} else if (ext == "bin") {
		loader.LoadSns(m_filepath.c_str());
	}

	return true;
}

void Symbol::LoadEE()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();
	tag = value["tag"].asString();
}

}