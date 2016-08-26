#ifndef _EASYTRAIL_SYMBOL_H_
#define _EASYTRAIL_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/TrailSymbol.h>

namespace etrail
{

class Symbol : public ee::Symbol, public s2::TrailSymbol
{
public:
	Symbol();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

}; // Symbol

}

#endif // _EASYTRAIL_SYMBOL_H_