#ifndef _LIBANIM2_SYMBOL_H_
#define _LIBANIM2_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Anim2Symbol.h>

namespace libanim2
{

class Symbol : public ee::Symbol, public s2::Anim2Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

}; // Symbol

}

#endif // _LIBANIM2_SYMBOL_H_