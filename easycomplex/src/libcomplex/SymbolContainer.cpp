#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Sprite.h>

#include <sprite2/Sprite.h>

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
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	return ee::ObjectVector<ee::Sprite>::Remove(sprites, static_cast<ee::Sprite*>(obj));
}

bool SymbolContainer::Insert(Object* obj)
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	return ee::ObjectVector<ee::Sprite>::Insert(sprites, static_cast<ee::Sprite*>(obj));
}

bool SymbolContainer::Insert(Object* obj, int idx)
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	return ee::ObjectVector<ee::Sprite>::Insert(sprites, static_cast<ee::Sprite*>(obj), idx);	
}

bool SymbolContainer::Clear()
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	return ee::ObjectVector<ee::Sprite>::Clear(sprites);
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	return ee::ObjectVector<ee::Sprite>::ResetOrder(sprites, 
		static_cast<const ee::Sprite*>(obj), up);
}

bool SymbolContainer::ResetOrderMost(const Object* obj, bool up)
{
	std::vector<ee::Sprite*> sprites;
	GetSprites(sprites);
	return ee::ObjectVector<ee::Sprite>::ResetOrderMost(sprites, 
		static_cast<const ee::Sprite*>(obj), up);
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