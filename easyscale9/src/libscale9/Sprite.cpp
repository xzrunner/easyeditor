#include "Sprite.h"
#include "Symbol.h"
#include "SpritePropertySetting.h"

namespace escale9
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::Scale9Sprite(spr)
	, ee::Sprite(spr)
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::Scale9Sprite(sym)
	, ee::Sprite(sym)
	, m_data(sym->GetScale9Data())
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	if (val["scale9"].isNull()) {
		return;
	}

	float w = val["scale9"]["width"].asDouble();
	float h = val["scale9"]["height"].asDouble();

	Symbol* sym = dynamic_cast<Symbol*>(m_sym);

	float sw, sh;
	sym->GetScale9Data().GetSize(sw, sh);
	if (sw != w || sh != h) {
		sym->ResizeScale9(w, h);
	}

	SetSize(w, h);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	float w, h;
	m_data.GetSize(w, h);
	val["scale9"]["width"] = w;
	val["scale9"]["height"] = h;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::GetSize(float& w, float& h) const
{
	m_data.GetSize(w, h);
}

void Sprite::SetSize(float w, float h)
{
	m_data.Resize(w, h);
}

void Sprite::Draw(const s2::RenderParams& params) const
{
	m_data.Draw(params);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}