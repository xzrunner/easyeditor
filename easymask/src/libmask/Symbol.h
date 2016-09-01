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

	void SetBase(s2::Sprite* base);
	void SetMask(s2::Sprite* mask);

protected:
	virtual void LoadResources();

}; // Symbol

}

#endif // _EASYMASK_SYMBOL_H_