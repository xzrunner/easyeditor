#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Sprite.h>

#include <sprite2/Sprite.h>

namespace ecomplex
{

SymbolContainer::SymbolContainer(std::shared_ptr<Symbol>& sym)
	: m_sym(sym)
{
}

void SymbolContainer::Traverse(ee::RefVisitor<ee::Sprite>& visitor, bool order/* = true*/) const
{
	std::vector<ee::SprPtr> sprs;
	GetSprites(sprs);
	ee::ObjectVector<ee::Sprite>::Traverse(sprs, visitor, order);
}

void SymbolContainer::Traverse(ee::RefVisitor<ee::Sprite>& visitor, ee::DataTraverseType type, bool order) const
{
	std::vector<ee::SprPtr> sprs;
	GetSprites(sprs);
	ee::ObjectVector<ee::Sprite>::Traverse(sprs, visitor, type, order);
}

bool SymbolContainer::Remove(const ee::SprPtr& obj)
{
	return m_sym->Remove(*obj);
}

bool SymbolContainer::Insert(const ee::SprPtr& obj)
{
	return m_sym->Add(obj);
}

bool SymbolContainer::Insert(const ee::SprPtr& obj, int idx)
{
	return m_sym->Add(obj, idx);
}

bool SymbolContainer::Clear()
{
	return m_sym->Clear();
}

bool SymbolContainer::ResetOrder(const ee::SprConstPtr& obj, bool up)
{
	return m_sym->ResetOrder(*obj, up);
}

bool SymbolContainer::ResetOrderMost(const ee::SprConstPtr& obj, bool up)
{
	return m_sym->ResetOrderMost(*obj, up);
}

bool SymbolContainer::Sort(std::vector<ee::SprPtr>& sprs)
{
	CU_VEC<s2::SprPtr> s2_sprites;
	s2_sprites.reserve(sprs.size());
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		s2_sprites.push_back(sprs[i]);
	}
	bool ret = m_sym->Sort(s2_sprites);
	sprs.clear();
	for (int i = 0, n = s2_sprites.size(); i < n; ++i) {
		sprs.push_back(std::dynamic_pointer_cast<ee::Sprite>(s2_sprites[i]));
	}
	return ret;
}

void SymbolContainer::GetSprites(std::vector<ee::SprPtr>& sprs) const
{
	auto& children = m_sym->GetAllChildren();
	sprs.reserve(children.size());
	for (int i = 0, n = children.size(); i < n; ++i) {
		sprs.push_back(std::dynamic_pointer_cast<ee::Sprite>(children[i]));
	}
}

}