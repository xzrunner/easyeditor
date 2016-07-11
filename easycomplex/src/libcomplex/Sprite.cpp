#include "Sprite.h"

#include <ee/SpriteFactory.h>

#include <sprite2/ComplexSprite.h>

namespace ecomplex
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_core = new s2::ComplexSprite(this);
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_core = new s2::ComplexSprite(*static_cast<s2::ComplexSprite*>(sprite.m_core), this);

	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::ComplexSprite(this);

	m_symbol->Retain();
	BuildBounding();	
}

Sprite::~Sprite()
{
	m_core->RemoveReference();

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

bool Sprite::Update(float dt) 
{ 
	bool ret = false;
	const std::vector<s2::Sprite*>& children = m_symbol->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
		if (child->Update(dt)) {
			ret = true;
		}

	}
	return ret; 
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

}