#ifndef _EASYRESPACKER_NODE_FROMI_LUA_H_
#define _EASYRESPACKER_NODE_FROMI_LUA_H_

namespace erespacker
{

class NodeFromLua
{
public:
	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);

}; // NodeFromLua

}

#endif // _EASYRESPACKER_NODE_FROMI_LUA_H_