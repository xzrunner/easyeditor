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
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

}; // Symbol

}

#endif // _EASYTRAIL_SYMBOL_H_