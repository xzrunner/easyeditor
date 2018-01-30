#include "Sprite.h"
#include "Symbol.h"
#include "SprPropSetting.h"

#include <s2loader/TrailSprLoader.h>

namespace etrail
{

Sprite::Sprite()
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::TrailSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::TrailSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::TrailSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& t_val = val["trail"];
	m_local = t_val["local"].asBool();

	s2loader::TrailSprLoader loader(std::dynamic_pointer_cast<s2::TrailSprite>(shared_from_this()));
	loader.LoadJson(val, dir);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value t_val;

	t_val["local"] = m_local;

	val["trail"] = t_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SprPropSetting(stage, std::dynamic_pointer_cast<etrail::Sprite>(shared_from_this()));
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}