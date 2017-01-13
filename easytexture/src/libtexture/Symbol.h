#ifndef _EASYTEXTURE_SYMBOL_H_
#define _EASYTEXTURE_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/TextureSymbol.h>

namespace etexture
{

class Symbol : public ee::Symbol, public s2::TextureSymbol
{
public:
	Symbol();
	virtual ~Symbol() {}

	static ee::Symbol* Create() { return new Symbol(); }
	
protected:
	virtual bool LoadResources();

}; // Symbol

}

#endif // _EASYTEXTURE_SYMBOL_H_