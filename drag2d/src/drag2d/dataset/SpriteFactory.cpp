#include "SpriteFactory.h"

#include "common/FileNameTools.h"
#include "common/FileNameParser.h"

#include "ImageSprite.h"
#include "FontBlankSprite.h"
#include "ScriptsSprite.h"

namespace d2d
{

SpriteFactory* SpriteFactory::m_instance = NULL;

SpriteFactory::CallbackMap SpriteFactory::m_creators;

SpriteFactory::SpriteFactory()
	: m_id(0)
{
}

ISprite* SpriteFactory::create(ISymbol* symbol)
{
	ISprite* sprite = NULL;

	wxString filepath = symbol->GetFilepath();
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
		wxString ext = FilenameTools::getExtension(filepath).Lower();

		if (ext == "png" || ext == "jpg" || ext == "bmp")
		{
			sprite = new ImageSprite(static_cast<ImageSymbol*>(symbol));
		}
		else if (ext == "json")
		{
			wxString type = d2d::FileNameParser::getFileTag(d2d::FileNameParser::getFileType(filepath));
			CallbackMap::iterator itr = m_creators.find(type.ToStdString());
			if (itr != m_creators.end()) {
				sprite = (itr->second)(symbol);
			} else if (FileNameParser::isType(filepath, FileNameParser::e_fontblank)) {
				sprite = new FontBlankSprite(static_cast<FontBlankSymbol*>(symbol));
			}
		}
		else if (ext == "lua")
		{
			if (FileNameParser::isType(filepath, FileNameParser::e_scripts)) {
				sprite = new ScriptsSprite(static_cast<ScriptsSymbol*>(symbol));
			}
		}
	}

	if (sprite) {
		insert(sprite);
		sprite->name = "_sprite"+wxString::FromDouble(m_id++);
	}

	if (!symbol->tag.empty()) {
		sprite->tag = symbol->tag;
	}

	return sprite;
}

void SpriteFactory::insert(ISprite* sprite)
{
	std::map<const ISymbol*, SpriteList>::iterator 
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

void SpriteFactory::remove(ISprite* sprite)
{
	std::map<const ISymbol*, SpriteList>::iterator 
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

void SpriteFactory::updateBoundings(const ISymbol& symbol)
{
	std::map<const ISymbol*, SpriteList>::iterator 
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
} // d2d