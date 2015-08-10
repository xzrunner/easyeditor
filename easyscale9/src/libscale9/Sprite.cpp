#include "Sprite.h"
#include "SpritePropertySetting.h"

namespace escale9
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: d2d::ISprite(sprite)
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
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	escale9::Symbol* sym = dynamic_cast<escale9::Symbol*>(symbol);
	if (!sym) {
		return;
	}
	d2d::obj_assign<escale9::Symbol>(m_symbol, sym);
}

void Sprite::Load(const Json::Value& val)
{
	ISprite::Load(val);

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
	ISprite::Store(val);

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
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans) const
{
	m_data.Draw(mt, mul, add, r_trans, g_trans, b_trans);
}

}