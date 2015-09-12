#ifndef _LIBCOMPLEX_PARTICLE3D_TO_SPR_H_
#define _LIBCOMPLEX_PARTICLE3D_TO_SPR_H_

#include <drag2d.h>
#include <easycoco.h>

struct p3d_ps_config;

namespace ecomplex
{

class Particle3DToSpr
{
public:
	static d2d::ISprite* Trans(const libcoco::PackParticle3D* p3d);

private:
	static p3d_ps_config* LoadConfig(const libcoco::PackParticle3D* p3d);

}; // Particle3DToSpr

}

#endif // _LIBCOMPLEX_PARTICLE3D_TO_SPR_H_