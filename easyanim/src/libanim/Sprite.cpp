#include "Sprite.h"
#include "Symbol.h"
#include "Utility.h"

#include <ee/std_functor.h>

namespace eanim
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::AnimSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::AnimSprite(sym)
	, ee::Sprite(sym)
{
}

bool Sprite::Update(const s2::RenderParams& params, float dt) 
{
	const Symbol* sym = dynamic_cast<const Symbol*>(m_sym);

	std::vector<ee::Sprite*> sprs;
	Utility::GetCurrSprites(sym, sym->GetCurrFrame(), sprs);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->Update(params, dt);
	}
	for_each(sprs.begin(), sprs.end(), ee::DeletePointerFunctor<ee::Sprite>());
	return true;
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}