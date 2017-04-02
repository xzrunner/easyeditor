#ifndef _EASYCOMPLEX_PARTICLE3D_TO_SPR_H_
#define _EASYCOMPLEX_PARTICLE3D_TO_SPR_H_

#include <easyrespacker.h>

namespace s2 { class P3dEmitterCfg; }

namespace ecomplex
{

class Particle3DToSpr
{
public:
	static ee::Sprite* Trans(const erespacker::PackParticle3D* p3d);

private:
	static s2::P3dEmitterCfg* LoadConfig(const erespacker::PackParticle3D* p3d);

}; // Particle3DToSpr

}

#endif // _EASYCOMPLEX_PARTICLE3D_TO_SPR_H_