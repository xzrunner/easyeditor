#include "SpriteFactory.h"
#include "FilepathHelper.h"
#include "SymFileType.h"
#include "SpriteIO.h"

#include "ImageSymbol.h"
#include <sprite2/ImageSprite.h>

#include <sprite2/Scale9Symbol.h>
#include <sprite2/Scale9Sprite.h>
#include "Scale9SymLoader.h"
#include "Scale9SprLoader.h"

#include "ComplexSymbol.h"
#include <sprite2/ComplexSprite.h>

namespace glue
{

SINGLETON_DEFINITION(SpriteFactory);

SpriteFactory::SpriteFactory()
{
}

s2::Sprite* SpriteFactory::Create(const std::string& filepath) const
{
	s2::Sprite* spr = NULL;
	SymFileType type = get_sym_file_type(filepath);
	switch (type)
	{
	case IMAGE:
		{
			ImageSymbol* sym = new ImageSymbol(filepath);
			spr = new s2::ImageSprite(sym);
			sym->RemoveReference();
		}
		break;
	case SCALE9:
		{
			s2::Scale9Symbol* sym = new s2::Scale9Symbol();
			Scale9SymLoader loader(sym);
			loader.LoadJson(filepath);
			spr = new s2::Scale9Sprite(sym);
			sym->RemoveReference();
		}
		break;
	case COMPLEX:
		{
			ComplexSymbol* sym = new ComplexSymbol();
			sym->LoadJson(filepath);
			spr = new s2::ComplexSprite(sym);
			sym->RemoveReference();
		}
		break;
	}
	return spr;
}

s2::Sprite* SpriteFactory::Create(const Json::Value& val, const std::string& dir) const
{
	s2::Sprite* spr = NULL;

	std::string filepath = val["filepath"].asString();
	filepath = FilepathHelper::Absolute(dir, filepath);

	spr = Create(filepath);
	if (!spr) {
		return NULL;
	}

	SpriteIO io(true, true);
	io.Load(val, spr);

	SymFileType type = get_sym_file_type(filepath);
	switch (type)
	{
	case SCALE9:
		{
			Scale9SprLoader loader(VI_DOWNCASTING<s2::Scale9Sprite*>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	}

	return spr;
}

}