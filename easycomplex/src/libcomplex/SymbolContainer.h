#ifndef _ECOMPLEX_SYMBOL_CONTAINER_H_
#define _ECOMPLEX_SYMBOL_CONTAINER_H_

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
	virtual void Traverse(d2d::IVisitor& visitor, bool order = true) const;
	virtual void Traverse(d2d::IVisitor& visitor, d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;
	virtual bool Remove(Object* obj);
	virtual void Insert(Object* obj);
	virtual void Clear();
	virtual bool ResetOrder(const Object* obj, bool up);

private:
	Symbol* m_symbol;

}; // SymbolContainer

}

#endif // _ECOMPLEX_SYMBOL_CONTAINER_H_