#include "ShapeFromBin.h"
#include "PackShape.h"
#include "pack_unpack.h"
#include "typedef.h"

#include <spritepack.h>

namespace erespacker
{

int ShapeFromBin::Size(const PackShape* shape)
{
	return SIZEOF_SHAPE + SizeVertices(shape->vertices);
}

void ShapeFromBin::Unpack(uint8_t** ptr, PackShape* shape)
{
	uint8_t shape_type;
	unpack(shape_type, ptr);
	
	uint32_t color;
	unpack(color, ptr);
	shape->color.FromRGBA(color);

	UnpackVertices(shape->vertices, ptr);
}

}