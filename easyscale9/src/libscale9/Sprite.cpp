#include "Sprite.h"
#include "SpritePropertySetting.h"

namespace escale9
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: d2d::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_data(sprite.m_data)
{
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_data(symbol->GetScale9Data())
{
	m_symbol->Retain();

	BuildBounding();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::Symbol* symbol)
{
	d2d::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	Sprite::Load(val);

	if (val["scale9"].isNull()) {
		return;
	}

	float w = val["scale9"]["width"].asDouble();
	float h = val["scale9"]["height"].asDouble();

	float sw, sh;
	m_symbol->GetScale9Data().GetSize(sw, sh);
	if (sw != w || sh != h)
	{
		m_symbol = m_symbol->Clone();
		m_symbol->ResizeScale9(w, h);
	}

	SetSize(w, h);
}

void Sprite::Store(Json::Value& val) const
{
	Sprite::Store(val);

	float w, h;
	m_data.GetSize(w, h);
	val["scale9"]["width"] = w;
	val["scale9"]["height"] = h;
}

d2d::IPropertySetting* Sprite::CreatePropertySetting(d2d::EditPanelImpl* stage)
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
	BuildBounding();
}

void Sprite::Draw(const d2d::Matrix& mt,
				  const d2d::ColorTrans& color) const
{
	m_data.Draw(mt, color);
}

}