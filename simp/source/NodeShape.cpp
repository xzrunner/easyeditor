#include "NodeShape.h"
#include "Allocator.h"
#include "ImportStream.h"
#include "simp_define.h"

namespace simp
{

NodeShape::NodeShape(Allocator& alloc, ImportStream& is)
{
	type		= is.UInt8();
	filling		= 1;	// todo
	color		= is.UInt32();
	// vertices
	vertices_n	= is.UInt16();
	alloc.Alloc(ALIGN_4BYTE(sizeof(uint16_t) * 2 * vertices_n));
	int idx = 0;
	for (int i = 0; i < vertices_n; ++i) {
		vertices[idx++] = is.UInt16();
		vertices[idx++] = is.UInt16();
	}
}

int NodeShape::Size()
{
	return sizeof(NodeShape) - sizeof(uint16_t);
}

}