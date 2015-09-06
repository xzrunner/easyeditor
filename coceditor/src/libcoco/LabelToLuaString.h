#ifndef _LIBCOCO_LABEL_TO_LUA_STRING_H_
#define _LIBCOCO_LABEL_TO_LUA_STRING_H_

#include "PackLabel.h"

namespace libcoco
{

class LabelToLuaString
{
public:
	static void Pack(const PackLabel* label, ebuilder::CodeGenerator& gen);

}; // LabelToLuaString

}

#endif // _LIBCOCO_LABEL_TO_LUA_STRING_H_