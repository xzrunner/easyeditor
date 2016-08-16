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
	m_symbol->AddReference();
}

SymbolContainer::~SymbolContainer()
{
	m_symbol->RemoveReference();
}

void SymbolContainer::Traverse(ee::Visitor<ee::Sprite>& visitor, bool order/* = true*/) const
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	ee::ObjectVector<ee::Sprite>::Traverse(sprites, visitor, order);
}

void SymbolContainer::Traverse(ee::Visitor<ee::Sprite>& visitor, ee::DataTraverseType type, bool order) const
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	ee::ObjectVector<ee::Sprite>::Traverse(sprites, visitor, type, order);
}

bool SymbolContainer::Remove(ee::Sprite* obj)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_symbol->Remove(spr);
}

bool SymbolContainer::Insert(ee::Sprite* obj)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_symbol->Add(spr);
}

bool SymbolContainer::Insert(ee::Sprite* obj, int idx)
{
	ee::Sprite* spr = static_cast<ee::Sprite*>(obj);
	return m_symbol->Add(spr, idx);
}

bool SymbolContainer::Clear()
{
	return m_symbol->Clear();
}

bool SymbolContainer::ResetOrder(const ee::Sprite* obj, bool up)
{
	const ee::Sprite* spr = static_cast<const ee::Sprite*>(obj);
	return m_symbol->ResetOrder(spr, up);
}

bool SymbolContainer::ResetOrderMost(const ee::Sprite* obj, bool up)
{
	const ee::Sprite* spr = static_cast<const ee::Sprite*>(obj);
	return m_symbol->ResetOrderMost(spr, up);
}

bool SymbolContainer::Sort(std::vector<ee::Sprite*>& sprites)
{
	std::vector<s2::Sprite*> s2_sprites;
	s2_sprites.reserve(sprites.size());
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		s2_sprites.push_back(sprites[i]);
	}
	bool ret = m_symbol->Sort(s2_sprites);
	sprites.clear();
	for (int i = 0, n = s2_sprites.size(); i < n; ++i) {
		sprites.push_back(dynamic_cast<ee::Sprite*>(s2_sprites[i]));
	}
	return ret;
}

void SymbolContainer::GetSprites(std::vector<ee::Sprite*>& sprites) const
{
	const std::vector<s2::Sprite*>& children = m_symbol->GetChildren();
	sprites.reserve(children.size());
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		sprites.push_back(child);
	}
}

}