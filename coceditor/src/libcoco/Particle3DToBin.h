#ifndef _LIBCOCO_PARTICLE3D_TO_BIN_H_
#define _LIBCOCO_PARTICLE3D_TO_BIN_H_

#include "PackParticle3D.h"

namespace libcoco
{

class Particle3DToBin
{
public:
	static int Size(const PackParticle3D* p3d);

	static void Pack(const PackParticle3D* p3d, uint8_t** ptr);

private:
	static int ComponentSize();

	static void PackComponent(const PackParticle3D::Component& comp, uint8_t** ptr);

	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static int TransBool(bool b);

}; // Particle3DToBin

}

#endif // _LIBCOCO_PARTICLE3D_TO_BIN_H_