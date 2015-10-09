#ifndef _LIBRESPACKER_LABEL_FROM_LUA_H_
#define _LIBRESPACKER_LABEL_FROM_LUA_H_

#include "PackLabel.h"

namespace librespacker
{

class LabelFromLua
{
public:
	static void Unpack(lua_State* L, PackLabel* label);

}; // LabelFromLua

}

#endif // _LIBRESPACKER_LABEL_FROM_LUA_H_