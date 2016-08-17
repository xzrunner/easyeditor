#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Shape.h>

namespace etexture
{

SymbolContainer::SymbolContainer(Symbol* sym)
{
	m_sym = sym;
	m_sym->AddReference();
}

SymbolContainer::~SymbolContainer()
{
	m_sym->RemoveReference();
}

void SymbolContainer::Traverse(ee::Visitor<ee::Shape>& visitor, bool order/* = true*/) const
{
	ee::ObjectVector<ee::Shape>::Traverse(m_sym->m_shapes, visitor, order);
}

void SymbolContainer::Traverse(ee::Visitor<ee::Shape>& visitor, ee::DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool SymbolContainer::Remove(ee::Shape* shape)
{
	return ee::ObjectVector<ee::Shape>::Remove(m_sym->m_shapes, shape);
}

bool SymbolContainer::Insert(ee::Shape* shape)
{
	return ee::ObjectVector<ee::Shape>::Insert(m_sym->m_shapes, shape);
}

bool SymbolContainer::Insert(ee::Shape* shape, int idx)
{
	return ee::ObjectVector<ee::Shape>::Insert(m_sym->m_shapes, shape, idx);
}

bool SymbolContainer::Clear()
{
	return ee::ObjectVector<ee::Shape>::Clear(m_sym->m_shapes);
}

bool SymbolContainer::ResetOrder(const ee::Shape* shape, bool up)
{
	return ee::ObjectVector<ee::Shape>::ResetOrder(m_sym->m_shapes, shape, up);
}

bool SymbolContainer::ResetOrderMost(const ee::Shape* shape, bool up)
{
	return ee::ObjectVector<ee::Shape>::ResetOrderMost(m_sym->m_shapes, shape, up);
}

}