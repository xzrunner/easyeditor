#include "Sprite.h"
#include "SpritePropertySetting.h"

#include <ee/SpriteFactory.h>

#include <sprite2/Scale9Sprite.h>

namespace escale9
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_core = new s2::Scale9Sprite(this);
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_data(sprite.m_data)
{
	m_core = new s2::Scale9Sprite(*static_cast<s2::Scale9Sprite*>(sprite.m_core));

	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_data(symbol->GetScale9Data())
{
	m_core = new s2::Scale9Sprite(this);

	m_symbol->Retain();
	BuildBounding();
}

Sprite::~Sprite()
{
	delete m_core; m_core = NULL;

	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	ee::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	if (val["scale9"].isNull()) {
		return;
	}

	float w = val["scale9"]["width"].asDouble();
	float h = val["scale9"]["height"].asDouble();

	float sw, sh;
	m_symbol->GetScale9Data().GetSize(sw, sh);
	if (sw != w || sh != h) {
		m_symbol->ResizeScale9(w, h);
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
	BuildBounding();
}

void Sprite::Draw(const s2::RenderParams& params) const
{
	m_data.Draw(params);
}

}