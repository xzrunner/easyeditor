#include "SpriteFactory.h"
#include "FilepathHelper.h"
#include "SymbolFileType.h"
#include "SpriteIO.h"

#include "ImageSymbol.h"
#include <sprite2/ImageSprite.h>
#include "Scale9Symbol.h"
#include <sprite2/Scale9Sprite.h>

namespace glue
{

SINGLETON_DEFINITION(SpriteFactory);

SpriteFactory::SpriteFactory()
{
}

s2::Sprite* SpriteFactory::Create(const std::string& filepath) const
{
	s2::Sprite* spr = NULL;
	SymbolFileType type = get_sym_file_type(filepath);
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
			Scale9Symbol* sym = new Scale9Symbol();
			sym->LoadJson(filepath);
			spr = new s2::Scale9Sprite(sym);
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

	if (spr) {
		SpriteIO io(true, true);
		io.Load(val, spr);
	}

	return spr;
}

//s2::Sprite* SpriteFactory::Create(s2::Symbol* sym) const
//{
//	return NULL;
//}

}