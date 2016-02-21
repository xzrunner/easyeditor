#ifndef _EASYRESPACKER_NODE_FROMI_BIN_H_
#define _EASYRESPACKER_NODE_FROMI_BIN_H_

#include <string>

namespace erespacker
{

class NodeFromBin
{
public:
	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);
	static bool TransBool(int b);

}; // NodeFromBin

}

#endif // _EASYRESPACKER_NODE_FROMI_BIN_H_