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

void SymbolContainer::traverse(d2d::IVisitor& visitor, bool order/* = true*/) const
{
	d2d::VectorUtils::traverse(m_symbol->m_shapes, visitor, order);
}

void SymbolContainer::traverse(d2d::IVisitor& visitor, d2d::TraverseType type, bool order) const
{
	traverse(visitor, order);
}

void SymbolContainer::remove(Object* obj)
{
	d2d::VectorUtils::remove(m_symbol->m_shapes, static_cast<d2d::IShape*>(obj));
}

void SymbolContainer::insert(Object* obj)
{
	d2d::VectorUtils::insert(m_symbol->m_shapes, static_cast<d2d::IShape*>(obj));
}

void SymbolContainer::clear()
{
	m_symbol->Release();
}

void SymbolContainer::resetOrder(const Object* obj, bool up)
{
	d2d::VectorUtils::resetOrder(m_symbol->m_shapes, static_cast<const d2d::IShape*>(obj), up);
}

}