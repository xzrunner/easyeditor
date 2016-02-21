#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Sprite.h>

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
	ee::ObjectVector<ee::Sprite>::Traverse(m_symbol->m_sprites, visitor, order);
}

void SymbolContainer::Traverse(ee::Visitor& visitor, ee::DataTraverseType type, bool order) const
{
	ee::ObjectVector<ee::Sprite>::Traverse(m_symbol->m_sprites, visitor, type, order);
}

bool SymbolContainer::Remove(Object* obj)
{
	return ee::ObjectVector<ee::Sprite>::Remove(m_symbol->m_sprites, static_cast<ee::Sprite*>(obj));
}

bool SymbolContainer::Insert(Object* obj)
{
	return ee::ObjectVector<ee::Sprite>::Insert(m_symbol->m_sprites, static_cast<ee::Sprite*>(obj));
}

bool SymbolContainer::Insert(Object* obj, int idx)
{
	return ee::ObjectVector<ee::Sprite>::Insert(m_symbol->m_sprites, static_cast<ee::Sprite*>(obj), idx);	
}

bool SymbolContainer::Clear()
{
	return ee::ObjectVector<ee::Sprite>::Clear(m_symbol->m_sprites);
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	return ee::ObjectVector<ee::Sprite>::ResetOrder(m_symbol->m_sprites, 
		static_cast<const ee::Sprite*>(obj), up);
}

bool SymbolContainer::ResetOrderMost(const Object* obj, bool up)
{
	return ee::ObjectVector<ee::Sprite>::ResetOrderMost(m_symbol->m_sprites, 
		static_cast<const ee::Sprite*>(obj), up);
}

}