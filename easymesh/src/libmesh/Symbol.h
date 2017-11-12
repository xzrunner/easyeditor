#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/MeshSymbol.h>

namespace emesh
{

class Mesh;
class Symbol : public ee::Symbol, public s2::MeshSymbol
{
public:
	Symbol();
	Symbol(const ee::SymPtr& base);

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	virtual bool LoadResources() override;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_