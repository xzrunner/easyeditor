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

void SymbolContainer::Traverse(d2d::IVisitor& visitor, bool order/* = true*/) const
{
	d2d::ObjectVector<d2d::ISprite>::Traverse(m_symbol->m_sprites, visitor, order);
}

void SymbolContainer::Traverse(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	d2d::ObjectVector<d2d::ISprite>::Traverse(m_symbol->m_sprites, visitor, type, order);
}

bool SymbolContainer::Remove(Object* obj)
{
	return d2d::ObjectVector<d2d::ISprite>::Remove(m_symbol->m_sprites, static_cast<d2d::ISprite*>(obj));
}

void SymbolContainer::Insert(Object* obj)
{
	d2d::ObjectVector<d2d::ISprite>::Insert(m_symbol->m_sprites, 
		static_cast<d2d::ISprite*>(obj));
}

void SymbolContainer::Clear()
{
	m_symbol->Release();

//		d2d::ObjectVector<d2d::ISprite>::clear(m_symbol->m_sprites);
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	return d2d::ObjectVector<d2d::ISprite>::ResetOrder(m_symbol->m_sprites, 
		static_cast<const d2d::ISprite*>(obj), up);
}

}