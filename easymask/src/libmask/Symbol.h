#ifndef _EASYMASK_SYMBOL_H_
#define _EASYMASK_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/MaskSymbol.h>

namespace emask
{

class Symbol : public ee::Symbol, public s2::MaskSymbol
{
public:

	// for p3d
	void Update(const s2::RenderParams& params, float dt);

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void UpdateP3DSymbol(s2::Symbol* sym, const s2::RenderParams& params, float dt);	

}; // Symbol

}

#endif // _EASYMASK_SYMBOL_H_