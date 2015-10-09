#ifndef _LIBCOMPLEX_PARTICLE2D_TO_SPR_H_
#define _LIBCOMPLEX_PARTICLE2D_TO_SPR_H_

#include <drag2d.h>
#include <easyrespacker.h>

struct p2d_ps_config;

namespace ecomplex
{

class Particle2DToSpr
{
public:
	static d2d::ISprite* Trans(const librespacker::PackParticle2D* p2d);

private:
	static p2d_ps_config* LoadConfig(const librespacker::PackParticle2D* p2d);

}; // Particle2DToSpr

}

#endif // _LIBCOMPLEX_PARTICLE2D_TO_SPR_H_