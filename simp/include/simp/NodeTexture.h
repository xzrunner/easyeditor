#ifndef _SIMP_NODE_TEXTURE_H_
#define _SIMP_NODE_TEXTURE_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeTexture : private cu::Uncopyable
{
public:
	uint32_t type;

public:
	NodeTexture(ImportStream& is);

	static int Size();

}; // NodeTexture

}

#endif // _SIMP_NODE_TEXTURE_H_