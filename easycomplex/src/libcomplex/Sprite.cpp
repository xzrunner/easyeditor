#include "Sprite.h"

namespace ecomplex
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
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

bool Sprite::Update(int version) 
{ 
	bool ret = false;
	for (int i = 0, n = m_symbol->m_sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = m_symbol->m_sprites[i];
		if (spr->Update(version)) {
			ret = true;
		}
	}
	return ret; 
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	Symbol* complex = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && complex)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = complex;
	}
}

}