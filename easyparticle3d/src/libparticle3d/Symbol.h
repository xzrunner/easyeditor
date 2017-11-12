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
	virtual void Traverse(ee::RefVisitor<ee::Sprite>& visitor) override;

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const override;

	virtual bool LoadResources() override;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_SYMBOL_H_
