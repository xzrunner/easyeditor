#ifndef _EASYCOMPLEX_PARTICLE3D_TO_SPR_H_
#define _EASYCOMPLEX_PARTICLE3D_TO_SPR_H_


#include <easyrespacker.h>

struct p3d_emitter_cfg;

namespace ecomplex
{

class Particle3DToSpr
{
public:
	static ee::Sprite* Trans(const erespacker::PackParticle3D* p3d);

private:
	static p3d_emitter_cfg* LoadConfig(const erespacker::PackParticle3D* p3d);

}; // Particle3DToSpr

}

#endif // _EASYCOMPLEX_PARTICLE3D_TO_SPR_H_