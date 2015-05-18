#include "SymbolContainer.h"
#include "Symbol.h"

namespace etexture
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
	d2d::ObjectVector<d2d::IShape>::Traverse(m_symbol->m_shapes, visitor, order);
}

void SymbolContainer::Traverse(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool SymbolContainer::Remove(Object* obj)
{
	return d2d::ObjectVector<d2d::IShape>::Remove(m_symbol->m_shapes, static_cast<d2d::IShape*>(obj));
}

void SymbolContainer::Insert(Object* obj)
{
	d2d::ObjectVector<d2d::IShape>::Insert(m_symbol->m_shapes, static_cast<d2d::IShape*>(obj));
}

void SymbolContainer::Clear()
{
	d2d::ObjectVector<d2d::IShape>::Clear(m_symbol->m_shapes);
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	return d2d::ObjectVector<d2d::IShape>::ResetOrder(m_symbol->m_shapes, static_cast<const d2d::IShape*>(obj), up);
}

}