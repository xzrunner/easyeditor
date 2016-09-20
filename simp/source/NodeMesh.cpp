#include "NodeMesh.h"
#include "Allocator.h"
#include "ImportStream.h"
#include "simp_define.h"

namespace simp
{

NodeMesh::NodeMesh(Allocator& alloc, ImportStream& is)
{
	base_id = is.UInt32();

	int idx = 0;
	outer_n = is.UInt16();
	outer = (uint16_t*)alloc.Alloc(ALIGN_4BYTE(sizeof(uint16_t) * 2 * outer_n));
	for (int i = 0; i < outer_n; ++i) {
		outer[idx++] = is.UInt16();
		outer[idx++] = is.UInt16();
	}

	idx = 0;
	inner_n = is.UInt16();
	inner = (uint16_t*)alloc.Alloc(ALIGN_4BYTE(sizeof(uint16_t) * 2 * inner_n));
	for (int i = 0; i < inner_n; ++i) {
		inner[idx++] = is.UInt16();
		inner[idx++] = is.UInt16();
	}
}

int NodeMesh::Size()
{
	return sizeof(NodeMesh) + PTR_SIZE_DIFF * 2;
}

}