#ifndef _SIMP_NODE_SHAPE_SPR_H_
#define _SIMP_NODE_SHAPE_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeShapeSpr : private cu::Uncopyable
{
public:
	uint32_t sym;

public:
	NodeShapeSpr(ImportStream& is);

	static int Size();

}; // NodeShapeSpr

}

#endif // _SIMP_NODE_SHAPE_SPR_H_