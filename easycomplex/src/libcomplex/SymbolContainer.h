#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class Symbol;

	class SymbolContainer : public d2d::IDataContainer
	{
	public:
		SymbolContainer(Symbol* symbol);
		virtual ~SymbolContainer();

		//
		// IDataContainer interface
		//
		virtual void traverse(d2d::IVisitor& visitor, bool order = true) const;
		virtual void traverse(d2d::IVisitor& visitor, d2d::TraverseType type = d2d::e_allExisting, bool order = true) const;
		virtual void remove(Object* obj);
		virtual void insert(Object* obj);
		virtual void clear();
		virtual void resetOrder(const Object* obj, bool up);

	private:
		Symbol* m_symbol;

	}; // SymbolContainer
}