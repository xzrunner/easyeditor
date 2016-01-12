#include "ShapeToBin.h"
#include "pack_unpack.h"
#include "typedef.h"

#include <spritepack.h>

namespace librespacker
{

int ShapeToBin::Size(const PackShape* shape)
{
	int sz = 0;
	sz += sizeof(uint16_t);		// id
	sz += sizeof(uint8_t);		// type
	sz += sizeof(uint8_t);		// shape type
	sz += sizeof(uint32_t);		// color
	sz += sizeof(uint16_t);		// vertices number
	sz += sizeof(int32_t) * 2 * shape->vertices.size();		// vertices
	return sz;
}

void ShapeToBin::Pack(const PackShape* shape, uint8_t** ptr)
{
	uint16_t id = shape->GetID();
	pack(id, ptr);

	uint8_t type = TYPE_SHAPE;
	pack(type, ptr);

	uint8_t shape_type = shape->type;
	pack(shape_type, ptr);

	uint32_t font_color = d2d::trans_color2int(shape->color, d2d::PT_RGBA);
	pack(font_color, ptr);

	if (shape->vertices.size() > USHRT_MAX) {
		throw d2d::Exception("ShapeToBin::Pack num too layer.");
	}
	uint16_t num = shape->vertices.size();
	pack(num, ptr);
	
	for (int i = 0, n = shape->vertices.size(); i < n; ++i) {
		const d2d::Vector& pos = shape->vertices[i];
		int32_t x = floor(pos.x * SCALE + 0.5f),
			    y =-floor(pos.y * SCALE + 0.5f);
		pack(x, ptr);
		pack(y, ptr);
	}
}

}