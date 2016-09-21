#include "NodeIcon.h"
#include "Allocator.h"
#include "ImportStream.h"
#include "simp_define.h"

namespace simp
{

NodeIcon::NodeIcon(Allocator& alloc, ImportStream& is)
{
	base_id = is.UInt32();
	type = is.UInt8();
	// vertices
	vertices_n	= is.UInt16();
	alloc.Alloc(ALIGN_4BYTE(sizeof(uint16_t) * 2 * vertices_n));
	int idx = 0;
	for (int i = 0; i < vertices_n; ++i) {
		vertices[idx++] = is.UInt16();
		vertices[idx++] = is.UInt16();
	}
}

int NodeIcon::Size()
{
	return sizeof(NodeIcon) - sizeof(uint16_t);
}

}