#ifndef _EASYRESPACKER_PARTICLE3D_FROM_BIN_H_
#define _EASYRESPACKER_PARTICLE3D_FROM_BIN_H_

#include "NodeFromBin.h"
#include "PackParticle3D.h"

namespace erespacker
{

class Particle3DFromBin : private NodeFromBin
{
public:
	static int Size(const PackParticle3D* p3d);

	static void Unpack(uint8_t** ptr, PackParticle3D* p3d);

private:
	static void UnpackComponent(uint8_t** ptr, PackParticle3D* p3d);

}; // Particle3DFromBin

}

#endif // _EASYRESPACKER_PARTICLE3D_FROM_BIN_H_