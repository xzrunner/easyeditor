#ifndef _LIBCOCO_LABEL_FROM_LUA_H_
#define _LIBCOCO_LABEL_FROM_LUA_H_

#include "PackLabel.h"

namespace libcoco
{

class LabelFromLua
{
public:
	static void Unpack(lua_State* L, PackLabel* label);

}; // LabelFromLua

}

#endif // _LIBCOCO_LABEL_FROM_LUA_H_