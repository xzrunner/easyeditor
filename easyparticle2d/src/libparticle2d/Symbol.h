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
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_SYMBOL_H_
