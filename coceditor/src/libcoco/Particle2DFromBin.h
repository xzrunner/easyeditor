#ifndef _LIBCOCO_PARTICLE2D_FROM_BIN_H_
#define _LIBCOCO_PARTICLE2D_FROM_BIN_H_

#include "PackParticle2D.h"

namespace libcoco
{

class Particle2DFromBin
{
public:
	static int Size(const PackParticle2D* p2d);

	static void Unpack(uint8_t** ptr, PackParticle2D* p2d);

private:
	static void UnpackComponent(uint8_t** ptr, PackParticle2D* p2d);
	
	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);
	static bool TransBool(int b);

}; // Particle2DFromBin

}

#endif // _LIBCOCO_PARTICLE2D_FROM_BIN_H_