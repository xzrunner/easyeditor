#ifndef _SIMP_NODE_TEXTURE_SPR_H_
#define _SIMP_NODE_TEXTURE_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeTextureSpr : private cu::Uncopyable
{
public:
	uint32_t sym;

public:
	NodeTextureSpr(ImportStream& is);

	static int Size();

}; // NodeTextureSpr

}

#endif // _SIMP_NODE_TEXTURE_SPR_H_