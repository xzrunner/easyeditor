#include "Sprite.h"
#include "Symbol.h"

#include <sprite2/UpdateParams.h>

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

bool Sprite::Update(const s2::UpdateParams& up) 
{ 
	bool dirty = false;

	Symbol* sym = dynamic_cast<Symbol*>(m_sym);

	if (sym->GetAnchorMgr().Update(up)) {
		dirty = true;
	}

	s2::UpdateParams up_child(up);
	up_child.Push(this);

	const std::vector<Sprite*>& sprs = sym->GetExtRefs();
	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		Sprite* child = sprs[i];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(up_child)) {
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