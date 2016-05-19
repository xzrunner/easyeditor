#ifndef _EASYRESPACKER_NODE_TO_LUA_H_
#define _EASYRESPACKER_NODE_TO_LUA_H_

#include <SM_Vector.h>

#include <vector>

namespace ebuilder { class CodeGenerator; }

namespace erespacker
{

class NodeToLua
{
public:
	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static std::string TransBool(bool b);

	static void PackVertices(const std::vector<sm::vec2>& vertices, ebuilder::CodeGenerator& gen);

}; // NodeToLua

}

#endif // _EASYRESPACKER_NODE_TO_LUA_H_