#ifndef _LIBRESPACKER_PARTICLE2D_TO_BIN_H_
#define _LIBRESPACKER_PARTICLE2D_TO_BIN_H_

#include "NodeToBin.h"
#include "PackParticle2D.h"

namespace librespacker
{

class Particle2DToBin : private NodeToBin
{
public:
	static int Size(const PackParticle2D* p2d);

	static void Pack(const PackParticle2D* p2d, uint8_t** ptr);

private:
	static int ComponentSize();

	static void PackComponent(const PackParticle2D::Component& comp, uint8_t** ptr);

}; // Particle2DToBin

}

#endif // _LIBRESPACKER_PARTICLE2D_TO_BIN_H_