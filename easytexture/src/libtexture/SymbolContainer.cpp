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

void SymbolContainer::Traverse(ee::Visitor& visitor, bool order/* = true*/) const
{
	ee::ObjectVector<ee::Shape>::Traverse(m_symbol->m_shapes, visitor, order);
}

void SymbolContainer::Traverse(ee::Visitor& visitor, ee::DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool SymbolContainer::Remove(Object* obj)
{
	return ee::ObjectVector<ee::Shape>::Remove(m_symbol->m_shapes, static_cast<ee::Shape*>(obj));
}

bool SymbolContainer::Insert(Object* obj)
{
	return ee::ObjectVector<ee::Shape>::Insert(m_symbol->m_shapes, static_cast<ee::Shape*>(obj));
}

bool SymbolContainer::Insert(Object* obj, int idx)
{
	return ee::ObjectVector<ee::Shape>::Insert(m_symbol->m_shapes, static_cast<ee::Shape*>(obj), idx);
}

bool SymbolContainer::Clear()
{
	return ee::ObjectVector<ee::Shape>::Clear(m_symbol->m_shapes);
}

bool SymbolContainer::ResetOrder(const Object* obj, bool up)
{
	return ee::ObjectVector<ee::Shape>::ResetOrder(m_symbol->m_shapes, static_cast<const ee::Shape*>(obj), up);
}

bool SymbolContainer::ResetOrderMost(const Object* obj, bool up)
{
	return ee::ObjectVector<ee::Shape>::ResetOrderMost(m_symbol->m_shapes, static_cast<const ee::Shape*>(obj), up);
}

}