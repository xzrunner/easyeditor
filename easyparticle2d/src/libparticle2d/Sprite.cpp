#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/Sprite.h>

#include <ps_2d.h>
#include <sprite2/Particle2d.h>

namespace eparticle2d
{

Sprite::Sprite()
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::Particle2dSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::Particle2dSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::Particle2dSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& p_val = val["particle2d"];
	SetLoop(p_val["loop"].asBool());
	SetLocalModeDraw(p_val["local_mode_draw"].asBool());
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value p_val;
	p_val["loop"] = GetLoop();
	p_val["local_mode_draw"] = GetLocalModeDraw();

	val["particle2d"] = p_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}