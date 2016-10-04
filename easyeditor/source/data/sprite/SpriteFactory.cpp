#include "SpriteFactory.h"
#include "FileHelper.h"
#include "SymbolFile.h"
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
#include "SymbolType.h"

#include <sprite2/SymType.h>

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
	int type = sym->Type();
	switch (type)
	{
	case s2::SYM_IMAGE:
		spr = new ImageSprite(dynamic_cast<ImageSymbol*>(sym));
		break;
	case SYM_FONTBLANK:
		spr = new FontBlankSprite(dynamic_cast<FontBlankSymbol*>(sym));
		break;
	case SYM_SCRIPTS:
		spr = new ScriptsSprite(dynamic_cast<ScriptsSymbol*>(sym));
		break;
	default:
		{
			CallbackMap::iterator itr = m_creators.find(type);
			if (itr != m_creators.end()) {
				spr = (itr->second)(sym);
			}
		}
	}

	if (spr) {
		Insert(spr);
		spr->SetName(NextName());
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
	const Symbol* sym = dynamic_cast<const Symbol*>(spr->GetSymbol());
	std::map<const Symbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.find(sym);
	if (itr != m_map_symbol2sprites.end()) 
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

std::string SpriteFactory::NextName() const
{
	return "_sprite" + StringHelper::ToString(m_id++);
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

void SpriteFactory::RegisterCreator(int type, CreateCallback cb)
{
	m_creators.insert(std::make_pair(type, cb));
}

void SpriteFactory::UnregisterCreator(int type)
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