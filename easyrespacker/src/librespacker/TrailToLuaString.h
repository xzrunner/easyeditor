#ifndef _EASYRESPACKER_TRAIL_TO_LUA_STRING_H_
#define _EASYRESPACKER_TRAIL_TO_LUA_STRING_H_

#include "NodeToLuaString.h"
#include "PackTrail.h"

namespace erespacker
{

class TrailToLuaString : private NodeToLuaString
{
public:
 	static void Pack(const PackTrail* trail, ebuilder::CodeGenerator& gen);
 
private:
	static void PackMT(const PackTrail* trail, ebuilder::CodeGenerator& gen);

	static void PackCompImages(const std::vector<PackTrail::CompImage>& comp_images, ebuilder::CodeGenerator& gen);	
	static void PackCompShapes(const std::vector<PackTrail::CompShape>& comp_shapes, ebuilder::CodeGenerator& gen);	

}; // TrailToLuaString

}

#endif // _EASYRESPACKER_TRAIL_TO_LUA_STRING_H_