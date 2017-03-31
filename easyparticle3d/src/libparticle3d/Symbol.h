#ifndef _EASYPARTICLE3D_SYMBOL_H_
#define _EASYPARTICLE3D_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Particle3dSymbol.h>

namespace eparticle3d
{

class Symbol : public ee::Symbol, public s2::Particle3dSymbol
{
public:
	Symbol();

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor);
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual bool LoadResources();

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_SYMBOL_H_
