#ifndef _SIMP_NODE_SHAPE_H_
#define _SIMP_NODE_SHAPE_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class NodeShape : private cu::Uncopyable
{
public:
	uint16_t type;
	uint16_t filling;

	uint32_t color;

	uint32_t vertices_n;
	uint16_t vertices[1];

public:
	NodeShape(Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeShape

}

#endif // _SIMP_NODE_SHAPE_H_