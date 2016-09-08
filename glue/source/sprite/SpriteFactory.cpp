#include "SpriteFactory.h"
#include "SymbolFactory.h"
#include "FilepathHelper.h"
#include "SymFileType.h"
#include "SpriteIO.h"

#include "Scale9SprLoader.h"
#include "TextboxLoader.h"

#include <sprite2/S2_Symbol.h>
#include <sprite2/ImageSprite.h>
#include <sprite2/Scale9Sprite.h>
#include <sprite2/TextboxSprite.h>
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
	s2::Symbol* sym = SymbolFactory::Instance()->Create(filepath);
	if (!sym) {
		return spr;
	}

	SymFileType type = get_sym_file_type(filepath);
	switch (type)
	{
	case IMAGE:
		spr = new s2::ImageSprite(sym);
		break;
	case SCALE9:
		spr = new s2::Scale9Sprite(sym);
		break;
	case TEXTBOX:
		spr = new s2::TextboxSprite(sym);
		break;
	case COMPLEX:
		spr = new s2::ComplexSprite(sym);
		break;
	}

	sym->RemoveReference();
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
	case TEXTBOX:
		{
			s2::TextboxSprite* text = VI_DOWNCASTING<s2::TextboxSprite*>(spr);
			text->SetText(val["text"]["text"].asString());
			TextboxLoader loader(text->GetTextbox());
			loader.LoadJson(val["text"]);
		}
		break;
	}

	return spr;
}

}