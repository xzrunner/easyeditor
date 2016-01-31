#include "SpriteFactory.h"
#include "FileHelper.h"
#include "FileType.h"
#include "ImageSprite.h"
#include "FontBlankSprite.h"
#include "ScriptsSprite.h"
#include "StringHelper.h"

namespace ee
{

SpriteFactory* SpriteFactory::m_instance = NULL;

SpriteFactory::CallbackMap SpriteFactory::m_creators;

SpriteFactory::SpriteFactory()
	: m_id(0)
{
}

Sprite* SpriteFactory::Create(Symbol* symbol)
{
	Sprite* sprite = NULL;

	const std::string& filepath = symbol->GetFilepath();
	if (filepath.empty())
	{
		// todo
// 		if (ecomplex::Symbol* s = dynamic_cast<ecomplex::Symbol*>(symbol))
// 			sprite = new ecomplex::Sprite(s);
// 		else if (libanim::Symbol* s = dynamic_cast<libanim::Symbol*>(symbol))
// 			sprite = new libanim::Sprite(s);
	}
	else
	{
		std::string ext = FileHelper::GetExtension(filepath);
		StringHelper::ToLower(ext);

		if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "pvr")
		{
			sprite = new ImageSprite(static_cast<ImageSymbol*>(symbol));
		}
		else if (ext == "json")
		{
			std::string type = FileType::GetTag(FileType::GetType(filepath));
			CallbackMap::iterator itr = m_creators.find(type);
			if (itr != m_creators.end()) {
				sprite = (itr->second)(symbol);
			} else if (FileType::IsType(filepath, FileType::e_fontblank)) {
				sprite = new FontBlankSprite(static_cast<FontBlankSymbol*>(symbol));
			}
		}
		else if (ext == "lua")
		{
			if (FileType::IsType(filepath, FileType::e_scripts)) {
				sprite = new ScriptsSprite(static_cast<ScriptsSymbol*>(symbol));
			}
		}
	}

	if (sprite) {
		Insert(sprite);
		sprite->name = std::string("_sprite") + StringHelper::ToString(m_id++);
	}

	if (!symbol->tag.empty()) {
		sprite->tag = "[symbol]";
	}

	return sprite;
}

void SpriteFactory::Insert(Sprite* sprite)
{
	std::map<const Symbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.find(&sprite->GetSymbol());
	if (itr == m_map_symbol2sprites.end())
	{
		SpriteList list;
		list.push_back(sprite);
		m_map_symbol2sprites.insert(std::make_pair(&sprite->GetSymbol(), list));
	}
	else 
	{
		itr->second.push_back(sprite);
	}
}

void SpriteFactory::Remove(Sprite* sprite)
{
	std::map<const Symbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.begin();
	for ( ; itr != m_map_symbol2sprites.end(); ++itr)
	{
		SpriteList::iterator itr_sprite = itr->second.begin();
		for ( ; itr_sprite != itr->second.end(); )
		{
			if (*itr_sprite == sprite)
				itr_sprite = itr->second.erase(itr_sprite);
			else
				++itr_sprite;
		}
	}
}

void SpriteFactory::UpdateBoundings(const Symbol& symbol)
{
	std::map<const Symbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.find(&symbol);
	if (itr != m_map_symbol2sprites.end())
	{
		for (size_t i = 0, n = itr->second.size(); i < n; ++i)
			itr->second[i]->BuildBounding();
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