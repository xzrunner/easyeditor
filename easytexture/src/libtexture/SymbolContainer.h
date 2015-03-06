#ifndef _EASYTEXTURE_SYMBOL_CONTAINER_H_
#define _EASYTEXTURE_SYMBOL_CONTAINER_H_

#include <drag2d.h>

namespace etexture
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
	virtual void remove(Object* obj);
	virtual void insert(Object* obj);
	virtual void clear();
	virtual void resetOrder(const Object* obj, bool up);

private:
	Symbol* m_symbol;

}; // SymbolContainer

}

#endif // _EASYTEXTURE_SYMBOL_CONTAINER_H_