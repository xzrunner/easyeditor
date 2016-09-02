#include "NodeLabel.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "Allocator.h"

namespace simp
{

NodeLabel::NodeLabel(Allocator& alloc, ImportStream& is)
{
	width		= is.UInt16();
	height		= is.UInt16();

	font		= is.UInt8();
	font_size	= is.UInt8();
	font_color	= is.RGBA();

	edge		= is.UInt8();
	edge_size	= is.UInt16() / 1024.0f;
	edge_color	= is.RGBA();

	align_hori	= is.UInt8();
	align_vert	= is.UInt8();

	space_hori	= is.UInt16() / 1024.0f;
	space_vert	= is.UInt16() / 1024.0f;

	uint8_t pack8 = is.UInt8();
	richtext = pack8 & 0x1;
	overflow = pack8 & 0x2;

	text		= is.String(alloc);
	tid			= is.String(alloc);
}

int NodeLabel::Size()
{
	return sizeof(NodeLabel) + PTR_SIZE_DIFF * 2;
}

}