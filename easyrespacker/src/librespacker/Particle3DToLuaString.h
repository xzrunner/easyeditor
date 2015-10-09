#ifndef _LIBRESPACKER_PARTICLE3D_TO_LUA_STRING_H_
#define _LIBRESPACKER_PARTICLE3D_TO_LUA_STRING_H_

#include "PackParticle3D.h"

namespace librespacker
{

class Particle3DToLuaString
{
public:
	static void Pack(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen);

private:
	static void PackPS(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen);
	static void PackComponent(const PackParticle3D::Component& comp, 
		ebuilder::CodeGenerator& gen);

	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static std::string TransBool(bool b);

}; // Particle3DToLuaString

}

#endif // _LIBRESPACKER_PARTICLE3D_TO_LUA_STRING_H_