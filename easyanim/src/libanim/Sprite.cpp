#include "Sprite.h"
#include "Symbol.h"
#include "Utility.h"

#include <ee/std_functor.h>

namespace eanim
{

Sprite::Sprite(Symbol* sym)
	: ee::Sprite(sym)
{
}

bool Sprite::Update(const s2::RenderParams& params, float dt) 
{
	const Symbol* sym = dynamic_cast<const Symbol*>(m_sym);

	std::vector<ee::Sprite*> sprites;
	Utility::GetCurrSprites(sym, sym->GetCurrFrame(), sprites);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Update(params, dt);
	}
	for_each(sprites.begin(), sprites.end(), ee::DeletePointerFunctor<ee::Sprite>());
	return true;
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}