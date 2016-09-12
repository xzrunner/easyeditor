#ifndef _SIMP_NODE_TRANS_H_
#define _SIMP_NODE_TRANS_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class NodeTrans
{
public:
	struct Color
	{
		uint32_t type;
		uint32_t color[1];

		static int Size() { return sizeof(Color) - sizeof(uint32_t); }
	};

	struct Geometry
	{
		uint32_t type;
		uint32_t geo[1];

		static int Size() { return sizeof(Geometry) - sizeof(uint32_t); }
	};

	struct Render
	{
		uint32_t type;
		uint32_t render[1];

		static int Size() { return sizeof(Render) - sizeof(uint32_t); }
	};

	Color col;
	Geometry geo;
	Render render;

}; // NodeTrans

}

#endif // _SIMP_NODE_TRANS_H_