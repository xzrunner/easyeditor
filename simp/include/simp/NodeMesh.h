#ifndef _SIMP_NODE_MESH_H_
#define _SIMP_NODE_MESH_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class NodeMesh : private cu::Uncopyable
{
public:
	uint32_t base_id;

	uint32_t outer_n;
	uint16_t* outer;

	uint32_t inner_n;
	uint16_t* inner;

public:
	NodeMesh(Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeMesh

}

#endif // _SIMP_NODE_MESH_H_