#ifndef _LIBRESPACKER_NODE_TO_LUA_H_
#define _LIBRESPACKER_NODE_TO_LUA_H_

#include <string>

namespace librespacker
{

class NodeToLua
{
public:
	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static std::string TransBool(bool b);

}; // NodeToLua

}

#endif // _LIBRESPACKER_NODE_TO_LUA_H_