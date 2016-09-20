#ifndef _SIMP_NODE_MESH_SPR_H_
#define _SIMP_NODE_MESH_SPR_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class NodeMeshSpr : private cu::Uncopyable
{
public:
	uint32_t mesh_id, base_id;

	uint32_t n;
	uint16_t vertices[1];

public:
	NodeMeshSpr(Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeMeshSpr

}

#endif // _SIMP_NODE_MESH_SPR_H_