#ifndef _LIBCOCO_PARTICLE2D_TO_LUA_STRING_H_
#define _LIBCOCO_PARTICLE2D_TO_LUA_STRING_H_

#include "PackParticle2D.h"

namespace libcoco
{

class Particle2DToLuaString
{
public:
	static void Pack(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen);

private:
	static void PackPS(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen);
	static void PackComponent(const PackParticle2D::Component& comp, 
		ebuilder::CodeGenerator& gen);

	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static std::string TransBool(bool b);

}; // Particle2DToLuaString

}

#endif // _LIBCOCO_PARTICLE2D_TO_LUA_STRING_H_