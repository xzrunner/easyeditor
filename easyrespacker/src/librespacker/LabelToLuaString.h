#ifndef _LIBRESPACKER_LABEL_TO_LUA_STRING_H_
#define _LIBRESPACKER_LABEL_TO_LUA_STRING_H_

#include "PackLabel.h"

namespace librespacker
{

class LabelToLuaString
{
public:
	static void Pack(const PackLabel* label, ebuilder::CodeGenerator& gen);

}; // LabelToLuaString

}

#endif // _LIBRESPACKER_LABEL_TO_LUA_STRING_H_