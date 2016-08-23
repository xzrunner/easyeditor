#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/MeshSymbol.h>

namespace ee { class Image; }

namespace emesh
{

class Mesh;
class Symbol : public ee::Symbol, public s2::MeshSymbol
{
public:
	Symbol();
	Symbol(ee::Symbol* base);

	void SetPause(bool pause) { m_pause = pause; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	bool m_pause;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_