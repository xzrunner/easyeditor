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

void SymbolContainer::Traverse(d2d::Visitor& visitor, bool order/* = true*/) const
{
	d2d::ObjectVector<d2d::Shape>::Traverse(m_symbol->m_shapes, visitor, order);
}

void SymbolContainer::Traverse(d2d::Visitor& visitor, d2d::DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool SymbolContainer::Remove(Object* obj)
{
	return d2d::ObjectVector<d2d::Shape>::Remove(m_symbol->m_shapes, static_cast<d2d::Shape*>(obj));
}

bool SymbolContainer::Insert(Object* obj)
{
	return d2d::ObjectVector<d2d::Shape>::Insert(m_symbol->m_shapes, static_cast<d2d::Shape*>(obj));
}

bool SymbolContainer::Insert(Object* obj, int idx)
{
	return d2d::ObjectVector<d2d::Shape>::Insert(m_symbol->m_shapes, static_cast<d2d::Shape*>(obj), idx);
}

bool SymbolContainer::Clear()
{
	return d2d::ObjectVector<d2d::Shape>::Clear(m_symbol->m_shapes);
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	return d2d::ObjectVector<d2d::Shape>::ResetOrder(m_symbol->m_shapes, static_cast<const d2d::Shape*>(obj), up);
}

bool SymbolContainer::ResetOrderMost(const Object* obj, bool up)
{
	return d2d::ObjectVector<d2d::Shape>::ResetOrderMost(m_symbol->m_shapes, static_cast<const d2d::Shape*>(obj), up);
}

}