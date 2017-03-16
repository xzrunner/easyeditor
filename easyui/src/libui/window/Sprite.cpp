#include "Sprite.h"
#include "Symbol.h"

namespace eui
{
namespace window
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, ee::Sprite(sym)
{
}

bool Sprite::Update(const s2::RenderParams& params) 
{ 
	bool dirty = false;

	Symbol* sym = dynamic_cast<Symbol*>(m_sym);

	if (sym->GetAnchorMgr().Update(params)) {
		dirty = true;
	}

	const std::vector<Sprite*>& sprs = sym->GetExtRefs();
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		if (sprs[i]->Update(params)) {
			dirty = true;
		}
	}

	return dirty; 
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}
}