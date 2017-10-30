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

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, ee::Sprite(sym)
{
}

bool Sprite::Update(const s2::UpdateParams& up) 
{ 
	bool dirty = false;

	auto sym = std::dynamic_pointer_cast<Symbol>(m_sym);

	if (sym->GetAnchorMgr().Update(up)) {
		dirty = true;
	}

	s2::UpdateParams up_child(up);
	up_child.Push(this);

	auto& sprs = sym->GetExtRefs();
	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		auto& child = sprs[i];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(up_child)) {
			dirty = true;
		}
	}

	return dirty; 
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}
}