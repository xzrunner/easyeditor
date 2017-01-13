#ifndef _EASYMASK_SYMBOL_H_
#define _EASYMASK_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/MaskSymbol.h>

namespace emask
{

class Symbol : public ee::Symbol, public s2::MaskSymbol
{
public:

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual bool LoadResources();

}; // Symbol

}

#endif // _EASYMASK_SYMBOL_H_