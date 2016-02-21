#ifndef _EASYRESPACKER_LABEL_TO_LUA_STRING_H_
#define _EASYRESPACKER_LABEL_TO_LUA_STRING_H_

#include "PackLabel.h"

namespace erespacker
{

class LabelToLuaString
{
public:
	static void Pack(const PackLabel* label, ebuilder::CodeGenerator& gen);

}; // LabelToLuaString

}

#endif // _EASYRESPACKER_LABEL_TO_LUA_STRING_H_