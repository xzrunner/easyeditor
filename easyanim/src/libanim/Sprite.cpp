#include "Sprite.h"
#include "Utility.h"

#include <ee/SpriteFactory.h>
#include <ee/std_functor.h>

#include <sprite2/AnimSprite.h>

namespace eanim
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_core = new s2::AnimSprite(this);
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_core = new s2::AnimSprite(*static_cast<s2::AnimSprite*>(sprite.m_core), this);

	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::AnimSprite(this);

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

bool Sprite::Update(const s2::RenderParams& params, float dt) 
{
	std::vector<ee::Sprite*> sprites;
	Utility::GetCurrSprites(m_symbol, m_symbol->GetCurrFrame(), sprites);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Update(params, dt);
	}
	for_each(sprites.begin(), sprites.end(), ee::DeletePointerFunctor<ee::Sprite>());
	return true;
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