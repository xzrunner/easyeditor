#ifndef _LIBRESPACKER_NODE_FROMI_LUA_H_
#define _LIBRESPACKER_NODE_FROMI_LUA_H_

namespace librespacker
{

class NodeFromLua
{
public:
	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);

}; // NodeFromLua

}

#endif // _LIBRESPACKER_NODE_FROMI_LUA_H_