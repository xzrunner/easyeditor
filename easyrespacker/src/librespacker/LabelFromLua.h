#ifndef _EASYRESPACKER_LABEL_FROM_LUA_H_
#define _EASYRESPACKER_LABEL_FROM_LUA_H_

#include "PackLabel.h"

namespace erespacker
{

class LabelFromLua
{
public:
	static void Unpack(lua_State* L, PackLabel* label);

}; // LabelFromLua

}

#endif // _EASYRESPACKER_LABEL_FROM_LUA_H_