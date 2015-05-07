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

	void SymbolContainer::traverse(d2d::IVisitor& visitor, bool order/* = true*/) const
	{
		d2d::VectorUtils::traverse(m_symbol->m_sprites, visitor, order);
	}

	void SymbolContainer::traverse(d2d::IVisitor& visitor, d2d::TraverseType type, bool order) const
	{
		d2d::VectorUtils::traverse(m_symbol->m_sprites, visitor, type, order);
	}

	void SymbolContainer::remove(Object* obj)
	{
		d2d::VectorUtils::remove(m_symbol->m_sprites, 
			static_cast<d2d::ISprite*>(obj));
	}

	void SymbolContainer::insert(Object* obj)
	{
		d2d::VectorUtils::insert(m_symbol->m_sprites, 
			static_cast<d2d::ISprite*>(obj));
	}

	void SymbolContainer::clear()
	{
		m_symbol->Release();

//		d2d::VectorUtils::clear(m_symbol->m_sprites);
	}

	void SymbolContainer::resetOrder(const Object* obj, bool up)
	{
		d2d::VectorUtils::resetOrder(m_symbol->m_sprites, 
			static_cast<const d2d::ISprite*>(obj), up);
	}
}