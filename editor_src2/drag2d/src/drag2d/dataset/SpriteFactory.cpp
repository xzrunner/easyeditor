#include "SpriteFactory.h"

#include "common/FileNameTools.h"
#include "common/FileNameParser.h"

#include "ImageSprite.h"
#include "ShapeSprite.h"
#include "EShapeSprite.h"
#include "MeshSprite.h"
#include "CombinationSprite.h"
#include "ComplexSprite.h"
#include "AnimSprite.h"
#include "Patch9Sprite.h"
#include "TextSprite.h"
#include "FontBlankSprite.h"

namespace d2d
{

SpriteFactory* SpriteFactory::m_instance = NULL;

ISprite* SpriteFactory::create(ISymbol* symbol)
{
	ISprite* sprite = NULL;

	wxString filepath = symbol->getFilepath();
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
			sprite = new ComplexSprite(static_cast<ComplexSymbol*>(symbol));
		else if (FileNameParser::isType(filepath, FileNameParser::e_anim))
			sprite = new AnimSprite(static_cast<AnimSymbol*>(symbol));
		else if (FileNameParser::isType(filepath, FileNameParser::e_9patch))
			sprite = new Patch9Sprite(static_cast<Patch9Symbol*>(symbol));
		else if (FileNameParser::isType(filepath, FileNameParser::e_fontblank))
			sprite = new FontBlankSprite(static_cast<FontBlankSymbol*>(symbol));
	}

	if (sprite) insert(sprite);

	return sprite;
}

void SpriteFactory::insert(ISprite* sprite)
{
	std::map<const ISymbol*, SpriteList>::iterator 
		itr = map_symbol2sprites_.find(&sprite->getSymbol());
	if (itr == map_symbol2sprites_.end())
	{
		SpriteList list;
		list.push_back(sprite);
		map_symbol2sprites_.insert(std::make_pair(&sprite->getSymbol(), list));
	}
	else 
	{
		itr->second.push_back(sprite);
	}
}

void SpriteFactory::remove(ISprite* sprite)
{
	std::map<const ISymbol*, SpriteList>::iterator 
		itr = map_symbol2sprites_.begin();
	for ( ; itr != map_symbol2sprites_.end(); ++itr)
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
		itr = map_symbol2sprites_.find(&symbol);
	if (itr != map_symbol2sprites_.end())
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