#include "SymbolContainer.h"
#include "Symbol.h"

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

void SymbolContainer::Traverse(d2d::Visitor& visitor, bool order/* = true*/) const
{
	d2d::ObjectVector<d2d::Sprite>::Traverse(m_symbol->m_sprites, visitor, order);
}

void SymbolContainer::Traverse(d2d::Visitor& visitor, d2d::DataTraverseType type, bool order) const
{
	d2d::ObjectVector<d2d::Sprite>::Traverse(m_symbol->m_sprites, visitor, type, order);
}

bool SymbolContainer::Remove(Object* obj)
{
	return d2d::ObjectVector<d2d::Sprite>::Remove(m_symbol->m_sprites, static_cast<d2d::Sprite*>(obj));
}

bool SymbolContainer::Insert(Object* obj)
{
	return d2d::ObjectVector<d2d::Sprite>::Insert(m_symbol->m_sprites, static_cast<d2d::Sprite*>(obj));
}

bool SymbolContainer::Insert(Object* obj, int idx)
{
	return d2d::ObjectVector<d2d::Sprite>::Insert(m_symbol->m_sprites, static_cast<d2d::Sprite*>(obj), idx);	
}

bool SymbolContainer::Clear()
{
	return d2d::ObjectVector<d2d::Sprite>::Clear(m_symbol->m_sprites);
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	return d2d::ObjectVector<d2d::Sprite>::ResetOrder(m_symbol->m_sprites, 
		static_cast<const d2d::Sprite*>(obj), up);
}

bool SymbolContainer::ResetOrderMost(const Object* obj, bool up)
{
	return d2d::ObjectVector<d2d::Sprite>::ResetOrderMost(m_symbol->m_sprites, 
		static_cast<const d2d::Sprite*>(obj), up);
}

}