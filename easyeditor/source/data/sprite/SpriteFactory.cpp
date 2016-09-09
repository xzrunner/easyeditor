#include "SpriteFactory.h"
#include "FileHelper.h"
#include "FileType.h"
#include "ImageSprite.h"
#include "ImageSymbol.h"
#include "FontBlankSprite.h"
#include "FontBlankSymbol.h"
#include "ScriptsSprite.h"
#include "ScriptsSymbol.h"
#include "StringHelper.h"
#include "SymbolSearcher.h"
#include "SymbolMgr.h"
#include "Exception.h"

namespace ee
{

SpriteFactory* SpriteFactory::m_instance = NULL;

SpriteFactory::CallbackMap SpriteFactory::m_creators;

SpriteFactory::SpriteFactory()
	: m_id(0)
{
}

Sprite* SpriteFactory::Create(Symbol* sym)
{
	Sprite* spr = NULL;

	const std::string& filepath = sym->GetFilepath();
	if (filepath.empty())
	{
		// todo
// 		if (ecomplex::Symbol* s = dynamic_cast<ecomplex::Symbol*>(sym))
// 			spr = new ecomplex::Sprite(s);
// 		else if (eanim::Symbol* s = dynamic_cast<eanim::Symbol*>(sym))
// 			spr = new eanim::Sprite(s);
	}
	else
	{
		std::string ext = FileHelper::GetExtension(filepath);
		StringHelper::ToLower(ext);

		if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "pvr" || ext == "pkm")
		{
			spr = new ImageSprite(dynamic_cast<ImageSymbol*>(sym));
		}
		else if (ext == "json")
		{
			std::string type = FileType::GetTag(FileType::GetType(filepath));
			CallbackMap::iterator itr = m_creators.find(type);
			if (itr != m_creators.end()) {
				spr = (itr->second)(sym);
			} else if (FileType::IsType(filepath, FileType::e_fontblank)) {
				spr = new FontBlankSprite(dynamic_cast<FontBlankSymbol*>(sym));
			}
		}
		else if (ext == "lua")
		{
			if (FileType::IsType(filepath, FileType::e_scripts)) {
				spr = new ScriptsSprite(dynamic_cast<ScriptsSymbol*>(sym));
			}
		}
	}

	if (spr) {
		Insert(spr);
		spr->SetName(std::string("_sprite") + StringHelper::ToString(m_id++));
	}

	if (!sym->tag.empty()) {
		spr->SetTag("[symbol]");
	}

	return spr;
}

Sprite* SpriteFactory::Create(const Json::Value& val, const std::string& dir)
{
	std::string filepath = SymbolSearcher::GetSymbolPath(dir, val);
	Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
	if (!sym) {
		std::string filepath = val["filepath"].asString();
		throw Exception("Symbol doesn't exist, \n[dir]:%s, \n[file]:%s !", 
			dir.c_str(), filepath.c_str());
	}
	SymbolSearcher::SetSymbolFilepaths(dir, sym, val);
	Sprite* spr = SpriteFactory::Instance()->Create(sym);
	spr->Load(val, dir);
	sym->RemoveReference();
	return spr;
}

void SpriteFactory::Insert(Sprite* spr)
{
	std::map<const Symbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.find(dynamic_cast<const Symbol*>(spr->GetSymbol()));
	if (itr == m_map_symbol2sprites.end())
	{
		SpriteList list;
		list.push_back(spr);
		m_map_symbol2sprites.insert(std::make_pair(dynamic_cast<const Symbol*>(spr->GetSymbol()), list));
	}
	else 
	{
		itr->second.push_back(spr);
	}
}

void SpriteFactory::Remove(Sprite* spr)
{
	std::map<const Symbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.begin();
	for ( ; itr != m_map_symbol2sprites.end(); ++itr)
	{
		SpriteList::iterator itr_sprite = itr->second.begin();
		for ( ; itr_sprite != itr->second.end(); )
		{
			if (*itr_sprite == spr)
				itr_sprite = itr->second.erase(itr_sprite);
			else
				++itr_sprite;
		}
	}
}

void SpriteFactory::UpdateBoundings(const Symbol& sym)
{
	std::map<const Symbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.find(&sym);
	if (itr != m_map_symbol2sprites.end())
	{
		for (size_t i = 0, n = itr->second.size(); i < n; ++i)
			itr->second[i]->UpdateBounding();
	}
}

void SpriteFactory::RegisterCreator(const std::string& type, CreateCallback cb)
{
	m_creators.insert(std::make_pair(type, cb));
}

void SpriteFactory::UnregisterCreator(const std::string& type)
{
	m_creators.erase(type);
}

SpriteFactory* SpriteFactory::Instance()
{
	if (!m_instance)
	{
		m_instance = new SpriteFactory();
	}
	return m_instance;
}
}