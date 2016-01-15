#ifndef _LIBRESPACKER_PARTICLE3D_TO_BIN_H_
#define _LIBRESPACKER_PARTICLE3D_TO_BIN_H_

#include "NodeToBin.h"
#include "PackParticle3D.h"

namespace librespacker
{

class Particle3DToBin : private NodeToBin
{
public:
	static int Size(const PackParticle3D* p3d);

	static void Pack(const PackParticle3D* p3d, uint8_t** ptr);

private:
	static int ComponentSize();

	static void PackComponent(const PackParticle3D::Component& comp, uint8_t** ptr);

}; // Particle3DToBin

}

#endif // _LIBRESPACKER_PARTICLE3D_TO_BIN_H_