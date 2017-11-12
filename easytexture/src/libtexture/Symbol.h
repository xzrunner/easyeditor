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

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }
	
protected:
	virtual bool LoadResources() override;

}; // Symbol

}

#endif // _EASYTEXTURE_SYMBOL_H_