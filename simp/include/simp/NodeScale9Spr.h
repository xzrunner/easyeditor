#ifndef _SIMP_NODE_SCALE9_SPR_H_
#define _SIMP_NODE_SCALE9_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeScale9Spr : private cu::Uncopyable
{
public:
	uint32_t sym;
	uint16_t width, height;

public:
	NodeScale9Spr(ImportStream& is);

	static int Size();	

}; // NodeScale9Spr

}

#endif // _SIMP_NODE_SCALE9_SPR_H_