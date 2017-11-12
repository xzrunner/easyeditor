#ifndef _EASYANIM3D_SYMBOL_H_
#define _EASYANIM3D_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/ModelSymbol.h>

namespace eanim3d
{

class Symbol : public ee::Symbol, public s2::ModelSymbol
{
public:
	Symbol();

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	virtual bool LoadResources() override;

}; // Symbol

}

#endif // _EASYANIM3D_SYMBOL_H_