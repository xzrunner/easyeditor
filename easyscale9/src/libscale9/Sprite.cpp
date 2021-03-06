#include "Sprite.h"
#include "Symbol.h"
#include "SpritePropertySetting.h"

#include <s2s/Scale9Spr.h>

namespace escale9
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::Scale9Sprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::Scale9Sprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::Scale9Sprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	if (!val["scale9"].isNull()) {
		float w = val["scale9"]["width"].asDouble();
		float h = val["scale9"]["height"].asDouble();
		m_s9.SetSize(w, h);
	}
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	sm::vec2 sz = m_s9.GetSize();
	val["scale9"]["width"] = sz.x;
	val["scale9"]["height"] = sz.y;
}

void Sprite::Load(const s2s::NodeSpr* spr)
{
	ee::Sprite::Load(spr);

	auto s9_spr = dynamic_cast<const s2s::Scale9Spr*>(spr);
	m_s9.SetSize(s9_spr->Width(), s9_spr->Height());
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}