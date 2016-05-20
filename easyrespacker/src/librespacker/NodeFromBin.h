#ifndef _EASYRESPACKER_NODE_FROMI_BIN_H_
#define _EASYRESPACKER_NODE_FROMI_BIN_H_

#include "typedef.h"

#include <SM_Vector.h>

#include <vector>

#include <stdint.h>

namespace erespacker
{

class NodeFromBin
{
public:
	static float TransTime(int time);
	static float TransDegree(int deg);
	static float TransFloatX100(int f);
	static bool  TransBool(int b);

	static int  SizeVertices(const std::vector<sm::vec2>& vertices);
	static void UnpackVertices(std::vector<sm::vec2>& vertices, 
		uint8_t** ptr, bool reverse_y = true, int scale = SCALE);

}; // NodeFromBin

}

#endif // _EASYRESPACKER_NODE_FROMI_BIN_H_