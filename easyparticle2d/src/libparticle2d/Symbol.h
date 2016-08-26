#ifndef _EASYPARTICLE2D_SYMBOL_H_
#define _EASYPARTICLE2D_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Particle2dSymbol.h>

namespace eparticle2d
{

class ParticleSystem;

class Symbol : public ee::Symbol, public s2::Particle2dSymbol
{
public:
	Symbol();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_SYMBOL_H_
