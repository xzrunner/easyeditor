#ifndef _SIMP_NODE_TRAIL_SPR_H_
#define _SIMP_NODE_TRAIL_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeTrailSpr : private cu::Uncopyable
{
public:
	uint32_t sym;

public:
	NodeTrailSpr(ImportStream& is);

	static int Size();

}; // NodeTrailSpr

}

#endif // _SIMP_NODE_TRAIL_SPR_H_