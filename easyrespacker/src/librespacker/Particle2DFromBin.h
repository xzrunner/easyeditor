#ifndef _EASYRESPACKER_PARTICLE2D_FROM_BIN_H_
#define _EASYRESPACKER_PARTICLE2D_FROM_BIN_H_

#include "NodeFromBin.h"
#include "PackParticle2D.h"

namespace erespacker
{

class Particle2DFromBin : private NodeFromBin
{
public:
	static int Size(const PackParticle2D* p2d);

	static void Unpack(uint8_t** ptr, PackParticle2D* p2d);

private:
	static void UnpackComponent(uint8_t** ptr, PackParticle2D* p2d);

}; // Particle2DFromBin

}

#endif // _EASYRESPACKER_PARTICLE2D_FROM_BIN_H_