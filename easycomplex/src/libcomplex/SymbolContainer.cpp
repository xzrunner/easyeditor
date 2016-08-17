#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Sprite.h>

#include <sprite2/S2_Sprite.h>

namespace ecomplex
{

SymbolContainer::SymbolContainer(Symbol* sym)
{
	m_sym = sym;
	m_sym->AddReference();
}

SymbolContainer::~SymbolContainer()
{
	m_sym->RemoveReference();
}

void SymbolContainer::Traverse(ee::Visitor<ee::Sprite>& visitor, bool order/* = true*/) const
{
	std::vector<ee::Sprite*> sprs;
	GetSprites(sprs);
	ee::ObjectVector<ee::Sprite>::Traverse(sprs, visitor, order);
}

void SymbolContainer::Traverse(ee::Visitor<ee::Sprite>& visitor, ee::DataTraverseType type, bool order) const
{
	std::vector<ee::Sprite*> sprs;
	GetSprites(sprs);
	ee::ObjectVector<ee::Sprite>::Traverse(sprs, visitor, type, order);
}

bool SymbolContainer::Remove(ee::Sprite* obj)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_sym->Remove(spr);
}

bool SymbolContainer::Insert(ee::Sprite* obj)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_sym->Add(spr);
}

bool SymbolContainer::Insert(ee::Sprite* obj, int idx)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_sym->Add(spr, idx);
}

bool SymbolContainer::Clear()
{
	return m_sym->Clear();
}

bool SymbolContainer::ResetOrder(const ee::Sprite* obj, bool up)
{
	const ee::Sprite* spr = static_cast<const ee::Sprite*>(obj);
	return m_sym->ResetOrder(spr, up);
}

bool SymbolContainer::ResetOrderMost(const ee::Sprite* obj, bool up)
{
	const ee::Sprite* spr = static_cast<const ee::Sprite*>(obj);
	return m_sym->ResetOrderMost(spr, up);
}

bool SymbolContainer::Sort(std::vector<ee::Sprite*>& sprs)
{
	std::vector<s2::Sprite*> s2_sprites;
	s2_sprites.reserve(sprs.size());
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		s2_sprites.push_back(sprs[i]);
	}
	bool ret = m_sym->Sort(s2_sprites);
	sprs.clear();
	for (int i = 0, n = s2_sprites.size(); i < n; ++i) {
		sprs.push_back(dynamic_cast<ee::Sprite*>(s2_sprites[i]));
	}
	return ret;
}

void SymbolContainer::GetSprites(std::vector<ee::Sprite*>& sprs) const
{
	const std::vector<s2::Sprite*>& children = m_sym->GetChildren();
	sprs.reserve(children.size());
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		sprs.push_back(child);
	}
}

}