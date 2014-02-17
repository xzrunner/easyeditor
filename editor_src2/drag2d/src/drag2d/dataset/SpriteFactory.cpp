#include "SpriteFactory.h"

#include "common/FileNameTools.h"
#include "common/FileNameParser.h"

#include "ImageSprite.h"
#include "ShapeSprite.h"
#include "EShapeSprite.h"
#include "MeshSprite.h"
#include "CombinationSprite.h"
#include "AnimSprite.h"
#include "Patch9Sprite.h"
#include "TextSprite.h"
//#include "FontBlankSprite.h"
#include "FontSprite.h"

#include <easycomplex.h>

namespace d2d
{

SpriteFactory* SpriteFactory::m_instance = NULL;

SpriteFactory::SpriteFactory()
	: m_id(0)
{
}

ISprite* SpriteFactory::create(ISymbol* symbol)
{
	ISprite* sprite = NULL;

	wxString filepath = symbol->getFilepath();
	if (filepath.empty())
	{
		if (complex::Symbol* s = dynamic_cast<complex::Symbol*>(symbol))
			sprite = new complex::Sprite(s);
	}
	else
	{
		wxString ext = FilenameTools::getExtension(filepath).Lower();

		if (ext == "png" || ext == "jpg" || ext == "bmp")
		{
			sprite = new ImageSprite(static_cast<ImageSymbol*>(symbol));
		}
		else if (ext == "ttf")
		{
			sprite = new TextSprite(static_cast<FontSymbol*>(symbol));
		}
		else if (ext == "txt")
		{
			if (FileNameParser::isType(filepath, FileNameParser::e_polygon))
				sprite = new ShapeSprite(static_cast<ShapeSymbol*>(symbol));
			else if (FileNameParser::isType(filepath, FileNameParser::e_mesh))
				sprite = new MeshSprite(static_cast<MeshSymbol*>(symbol));
			else if (FileNameParser::isType(filepath, FileNameParser::e_combination))
				sprite = new CombinationSprite(static_cast<CombinationSymbol*>(symbol));
		}
		else if (ext == "json")
		{
			if (FileNameParser::isType(filepath, FileNameParser::e_shape))
				sprite = new EShapeSprite(static_cast<EShapeSymbol*>(symbol));
			else if (FileNameParser::isType(filepath, FileNameParser::e_complex))
				sprite = new complex::Sprite(static_cast<complex::Symbol*>(symbol));
			else if (FileNameParser::isType(filepath, FileNameParser::e_anim))
				sprite = new AnimSprite(static_cast<AnimSymbol*>(symbol));
			else if (FileNameParser::isType(filepath, FileNameParser::e_9patch))
				sprite = new Patch9Sprite(static_cast<Patch9Symbol*>(symbol));
			else if (FileNameParser::isType(filepath, FileNameParser::e_fontblank))
				//sprite = new FontBlankSprite(static_cast<FontBlankSymbol*>(symbol));
				sprite = new FontSprite(static_cast<FontBlankSymbol*>(symbol));
		}
	}

	if (sprite) insert(sprite);

	sprite->name = "_sprite"+wxString::FromDouble(m_id++);

	return sprite;
}

void SpriteFactory::insert(ISprite* sprite)
{
	std::map<const ISymbol*, SpriteList>::iterator 
		itr = m_map_symbol2sprites.find(&sprite->getSymbol());
	if (itr == m_map_symbol2sprites.end())
	{
		SpriteList list;
		list.push_back(sprite);
		m_map_symbol2sprites.insert(std::make_pair(&sprite->getSymbol(), list));
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
			itr->second[i]->buildBounding();
	}
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