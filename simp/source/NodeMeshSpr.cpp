#include "NodeMeshSpr.h"
#include "Allocator.h"
#include "ImportStream.h"
#include "simp_define.h"

namespace simp
{

NodeMeshSpr::NodeMeshSpr(Allocator& alloc, ImportStream& is)
{
	mesh_id = is.UInt32();
	base_id	= is.UInt32();

	n = is.UInt16();
	void* ptr = alloc.Alloc(ALIGN_4BYTE(sizeof(uint16_t) * 2 * n));
	int idx = 0;
	for (int i = 0; i < n; ++i) {
		vertices[idx++] = is.UInt16();
		vertices[idx++] = is.UInt16();
	}
}

int NodeMeshSpr::Size()
{
	return sizeof(NodeMeshSpr) - sizeof(uint16_t);
}

}