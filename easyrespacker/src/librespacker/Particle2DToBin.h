#ifndef _LIBRESPACKER_PARTICLE2D_TO_BIN_H_
#define _LIBRESPACKER_PARTICLE2D_TO_BIN_H_

#include "PackParticle2D.h"

namespace librespacker
{

class Particle2DToBin
{
public:
	static int Size(const PackParticle2D* p2d);

	static void Pack(const PackParticle2D* p2d, uint8_t** ptr);

private:
	static int ComponentSize();

	static void PackComponent(const PackParticle2D::Component& comp, uint8_t** ptr);

	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static int TransBool(bool b);

}; // Particle2DToBin

}

#endif // _LIBRESPACKER_PARTICLE2D_TO_BIN_H_