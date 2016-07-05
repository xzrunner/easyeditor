#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Sprite.h>

#include <sprite2/S2_Sprite.h>

namespace ecomplex
{

SymbolContainer::SymbolContainer(Symbol* symbol)
{
	m_symbol = symbol;
	m_symbol->Retain();
}

SymbolContainer::~SymbolContainer()
{
	m_symbol->Release();
}

void SymbolContainer::Traverse(ee::Visitor& visitor, bool order/* = true*/) const
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	ee::ObjectVector<ee::Sprite>::Traverse(sprites, visitor, order);
}

void SymbolContainer::Traverse(ee::Visitor& visitor, ee::DataTraverseType type, bool order) const
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	ee::ObjectVector<ee::Sprite>::Traverse(sprites, visitor, type, order);
}

bool SymbolContainer::Remove(Object* obj)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_symbol->Remove(spr);
}

bool SymbolContainer::Insert(Object* obj)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_symbol->Add(spr);
}

bool SymbolContainer::Insert(Object* obj, int idx)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_symbol->Add(spr, idx);
}

bool SymbolContainer::Clear()
{
	return m_symbol->Clear();
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	const ee::Sprite* spr = static_cast<const ee::Sprite*>(obj);
	return m_symbol->ResetOrder(spr, up);
}

bool SymbolContainer::ResetOrderMost(const Object* obj, bool up)
{
	const ee::Sprite* spr = static_cast<const ee::Sprite*>(obj);
	return m_symbol->ResetOrderMost(spr, up);
}

bool SymbolContainer::Sort(std::vector<ee::Sprite*>& sprites)
{
	return m_symbol->Sort(sprites);
}

void SymbolContainer::GetSprites(std::vector<ee::Sprite*>& sprites) const
{
	const std::vector<s2::Sprite*>& children = m_symbol->GetChildren();
	sprites.reserve(children.size());
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
		sprites.push_back(child);
	}
}

}