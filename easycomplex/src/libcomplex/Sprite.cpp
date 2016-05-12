#include "Sprite.h"

#include <ee/SpriteFactory.h>

#include <sprite2/ComplexSprite.h>

namespace ecomplex
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_core = new s2::ComplexSprite;
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_core = new s2::ComplexSprite;

	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::ComplexSprite;

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

bool Sprite::Update(float dt) 
{ 
	bool ret = false;
	for (int i = 0, n = m_symbol->m_sprites.size(); i < n; ++i) {
		ee::Sprite* spr = m_symbol->m_sprites[i];
		if (spr->Update(dt)) {
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