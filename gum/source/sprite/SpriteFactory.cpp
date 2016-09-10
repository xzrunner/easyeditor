#include "SpriteFactory.h"
#include "SymbolFactory.h"
#include "FilepathHelper.h"
#include "SymFileType.h"
#include "SpriteIO.h"

#include "Scale9SprLoader.h"
#include "TextboxLoader.h"
#include "ComplexSprLoader.h"
#include "AnimSprLoader.h"
#include "P3dSprLoader.h"
#include "P2dSprLoader.h"
#include "MeshSprLoader.h"

#include <sprite2/S2_Symbol.h>
#include <sprite2/ImageSprite.h>
#include <sprite2/Scale9Sprite.h>
#include <sprite2/TextureSprite.h>
#include <sprite2/TextboxSprite.h>
#include <sprite2/ComplexSprite.h>
#include <sprite2/AnimSprite.h>
#include <sprite2/Particle3dSprite.h>
#include <sprite2/Particle2dSprite.h>
#include <sprite2/MeshSprite.h>
#include <sprite2/MaskSprite.h>
#include <sprite2/TrailSprite.h>

namespace gum
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
	case TEXTURE:
		spr = new s2::TextureSprite(sym);
		break;
	case TEXTBOX:
		spr = new s2::TextboxSprite(sym);
		break;
	case COMPLEX:
		spr = new s2::ComplexSprite(sym);
		break;
	case ANIMATION:
		spr = new s2::AnimSprite(sym);
		break;
	case PARTICLE3D:
		spr = new s2::Particle3dSprite(sym);
		break;
	case PARTICLE2D:
		spr = new s2::Particle2dSprite(sym);
		break;
	case MESH:
		spr = new s2::MeshSprite(sym);
		break;
	case MASK:
		spr = new s2::MaskSprite(sym);
		break;
	case TRAIL:
		spr = new s2::TrailSprite(sym);
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
	case COMPLEX:
		{
			ComplexSprLoader loader(VI_DOWNCASTING<s2::ComplexSprite*>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case ANIMATION:
		{
			AnimSprLoader loader(VI_DOWNCASTING<s2::AnimSprite*>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case PARTICLE3D:
		{
			P3dSprLoader loader(VI_DOWNCASTING<s2::Particle3dSprite*>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case PARTICLE2D:
		{
			P2dSprLoader loader(VI_DOWNCASTING<s2::Particle2dSprite*>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case MESH:
		{
			MeshSprLoader loader(VI_DOWNCASTING<s2::MeshSprite*>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case TRAIL:
		{
			//
		}
		break;
	}

	return spr;
}

}