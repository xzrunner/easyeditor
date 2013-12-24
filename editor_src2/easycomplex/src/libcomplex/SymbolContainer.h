#pragma once

#include <drag2d.h>

namespace libcomplex
{
	class SymbolContainer : public d2d::IDataContainer
	{
	public:
		SymbolContainer(d2d::ComplexSymbol* symbol);
		virtual ~SymbolContainer();

		//
		// IDataContainer interface
		//
		virtual void traverse(d2d::IVisitor& visitor, bool order = true) const;
		virtual void remove(Object* obj);
		virtual void insert(Object* obj);
		virtual void clear();
		virtual void resetOrder(const Object* obj, bool up);

	private:
		d2d::ComplexSymbol* m_symbol;

	}; // SymbolContainer
}