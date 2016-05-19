#include "ShapeToBin.h"
#include "PackShape.h"
#include "pack_unpack.h"
#include "typedef.h"
#include "NodeToBin.h"

// for TYPE_SHAPE
#include <spritepack.h>

namespace erespacker
{

int ShapeToBin::Size(const PackShape* shape)
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// shape type
	sz += sizeof(uint32_t);			// color
	SizeVertices(shape->vertices);	// vertices
	return sz;
}

void ShapeToBin::Pack(const PackShape* shape, uint8_t** ptr)
{
	uint16_t id = shape->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_SHAPE;
	pack(type, ptr);

	uint8_t shape_type = shape->type;
	pack(shape_type, ptr);

	uint32_t font_color = shape->color.ToRGBA();
	pack(font_color, ptr);

	PackVertices(shape->vertices, ptr);
}

}