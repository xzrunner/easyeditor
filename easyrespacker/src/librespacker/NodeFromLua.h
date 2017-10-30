#ifndef _EASYRESPACKER_NODE_FROMI_LUA_H_
#define _EASYRESPACKER_NODE_FROMI_LUA_H_

#include "typedef.h"

#include <SM_Vector.h>
#include <cu/cu_stl.h>

extern "C" {
#include <lua.h>
};

namespace erespacker
{

class NodeFromLua
{
public:
	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);

	static void UnpackVertices(CU_VEC<sm::vec2>& vertices, lua_State* L, 
		const std::string& name, bool reverse_y = true, int scale = SCALE);

}; // NodeFromLua

}

#endif // _EASYRESPACKER_NODE_FROMI_LUA_H_