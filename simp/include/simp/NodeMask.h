#ifndef _SIMP_NODE_MASK_H_
#define _SIMP_NODE_MASK_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeMask : private cu::Uncopyable
{
public:
	uint32_t base_id;
	uint32_t mask_id;

public:
	NodeMask(ImportStream& is);

	static int Size();

}; // NodeMask

}

#endif // _SIMP_NODE_MASK_H_