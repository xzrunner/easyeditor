#ifndef _SIMP_NODE_MASK_SPR_H_
#define _SIMP_NODE_MASK_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeMaskSpr : private cu::Uncopyable
{
public:
	uint32_t sym;

public:
	NodeMaskSpr(ImportStream& is);

	static int Size();

}; // NodeMaskSpr

}

#endif // _SIMP_NODE_MASK_SPR_H_