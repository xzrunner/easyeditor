#include "SpriteFactory.h"
#include "SymbolFactory.h"
#include "FilepathHelper.h"
#include "SymFileType.h"
#include "SpriteIO.h"

#include "Scale9SprLoader.h"
#include "TextboxSprLoader.h"
#include "ComplexSprLoader.h"
#include "AnimSprLoader.h"
#include "P3dSprLoader.h"
#include "P2dSprLoader.h"
#include "MeshSprLoader.h"

#include <simp/simp_types.h>
#include <simp/NodeFactory.h>
#include <simp/NodeScale9Spr.h>
#include <simp/NodeComplexSpr.h>
#include <simp/NodeAnimationSpr.h>

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

#include <assert.h>

namespace gum
{

SINGLETON_DEFINITION(SpriteFactory);

SpriteFactory::SpriteFactory()
{
}

s2::Sprite* SpriteFactory::Create(s2::Symbol* sym, SymFileType type) const
{
	s2::Sprite* spr = NULL;
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
	default:
		assert(0);
	}
	return spr;
}

s2::Sprite* SpriteFactory::Create(const std::string& filepath, SymFileType* type) const
{
	SymFileType _type;
	s2::Symbol* sym = SymbolFactory::Instance()->Create(filepath, &_type);
	if (!sym) {
		return NULL;
	}

	if (type) {
		*type = _type;
	}

	s2::Sprite* spr = Create(sym, _type);
	sym->RemoveReference();
	return spr;
}

s2::Sprite* SpriteFactory::Create(const Json::Value& val, const std::string& dir) const
{
	s2::Sprite* spr = NULL;

	std::string filepath = val["filepath"].asString();
	filepath = FilepathHelper::Absolute(dir, filepath);

	SymFileType type;
	spr = Create(filepath, &type);
	if (!spr) {
		return NULL;
	}

	SpriteIO io(true, true);
	io.Load(val, spr);

	switch (type)
	{
	case IMAGE:
		{
		}
		break;
	case SCALE9:
		{
			Scale9SprLoader loader(VI_DOWNCASTING<s2::Scale9Sprite*>(spr));
			loader.LoadJson(val, dir);
		}
		break;
	case TEXTURE:
		{
		}
		break;
	case TEXTBOX:
		{
			s2::TextboxSprite* text = VI_DOWNCASTING<s2::TextboxSprite*>(spr);
			TextboxSprLoader loader(text);
			loader.LoadJson(val);
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
	case MASK:
		{
		}
		break;
	case TRAIL:
		{
		}
		break;
	default:
		assert(0);
	}

	return spr;
}

s2::Sprite* SpriteFactory::Create(uint32_t id)
{
	s2::Sprite* spr = NULL;

	int type;
	const void* data = simp::NodeFactory::Instance()->Create(id, &type);
	assert(data);
	switch (type)
	{
	case simp::TYPE_IMAGE:
		{
			s2::Symbol* sym = SymbolFactory::Instance()->Create(id);
			spr = new s2::ImageSprite(sym);
			sym->RemoveReference();
		}
		break;
	case simp::TYPE_SCALE9_SPR:
		{
			const simp::NodeScale9Spr* node = (const simp::NodeScale9Spr*)data;

			s2::Symbol* sym = SymbolFactory::Instance()->Create(node->sym);
			s2::Scale9Sprite* s9_spr = new s2::Scale9Sprite(sym);
			sym->RemoveReference();

			Scale9SprLoader loader(s9_spr);
			loader.LoadBin(node);

			spr = s9_spr;
		}
		break;
	case simp::TYPE_LABEL:
		{
			const simp::NodeLabel* node = (const simp::NodeLabel*)data;

			s2::Symbol* sym = SymbolFactory::Instance()->Create(id);
			s2::TextboxSprite* tb_spr = VI_DOWNCASTING<s2::TextboxSprite*>(SpriteFactory::Instance()->Create(sym, TEXTBOX));
			sym->RemoveReference();

			TextboxSprLoader loader(tb_spr);
			loader.LoadBin(node);

			spr = tb_spr;
		}
		break;
	case simp::TYPE_COMPLEX_SPR:
		{
			const simp::NodeComplexSpr* node = (const simp::NodeComplexSpr*)data;

			s2::Symbol* sym = SymbolFactory::Instance()->Create(node->sym);
			s2::ComplexSprite* comp_spr = VI_DOWNCASTING<s2::ComplexSprite*>(SpriteFactory::Instance()->Create(sym, COMPLEX));
			sym->RemoveReference();

			ComplexSprLoader loader(comp_spr);
			loader.LoadBin(node);

			spr = comp_spr;
		}
		break;
	case simp::TYPE_ANIMATION_SPR:
		{
			const simp::NodeAnimationSpr* node = (const simp::NodeAnimationSpr*)data;

			s2::Symbol* sym = SymbolFactory::Instance()->Create(node->sym);
			s2::AnimSprite* anim_spr = VI_DOWNCASTING<s2::AnimSprite*>(SpriteFactory::Instance()->Create(sym, ANIMATION));
			sym->RemoveReference();

			AnimSprLoader loader(anim_spr);
			loader.LoadBin(node);

			spr = anim_spr;
		}
		break;
	default:
		assert(0);
	}

	return spr;
}

}