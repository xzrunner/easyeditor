#ifndef _LIBRESPACKER_PARTICLE3D_FROM_BIN_H_
#define _LIBRESPACKER_PARTICLE3D_FROM_BIN_H_

#include "PackParticle3D.h"

namespace librespacker
{

class Particle3DFromBin
{
public:
	static int Size(const PackParticle3D* p3d);

	static void Unpack(uint8_t** ptr, PackParticle3D* p3d);

private:
	static void UnpackComponent(uint8_t** ptr, PackParticle3D* p3d);
	
	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);
	static bool TransBool(int b);

}; // Particle3DFromBin

}

#endif // _LIBRESPACKER_PARTICLE3D_FROM_BIN_H_