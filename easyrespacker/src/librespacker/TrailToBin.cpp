#include "TrailToBin.h"
#include "PackTrail.h"
#include "NodeToBin.h"
#include "pack_unpack.h"

// for TYPE_TRAIL
#include <spritepack.h>
#include <mt_2d.h>

#include <limits>

namespace erespacker
{

int TrailToBin::Size(const PackTrail* trail)
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// mode
	sz += sizeof(uint16_t);			// count
	sz += sizeof(uint16_t);			// life_begin
	sz += sizeof(uint16_t);			// life_offset
	sz += sizeof(uint16_t);			// fadeout_time
	sz += sizeof(uint16_t);			// component size
	if (trail->mode == T2D_MODE_IMAGE) {
		sz += CompImageSize() * trail->comp_images.size();
	} else {
		assert(trail->mode == T2D_MODE_SHAPE);
		sz += CompShapeSize() * trail->comp_shapes.size();
	}
	return sz;
}

void TrailToBin::Pack(const PackTrail* trail, uint8_t** ptr)
{
	uint16_t id = trail->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_TRAIL;
	pack(type, ptr);
 
	uint8_t mode = trail->mode;
	pack(mode, ptr);

	uint16_t count = trail->count;
	pack(count, ptr);

	uint16_t life_begin = TransTime(trail->life_begin);
	pack(life_begin, ptr);
	uint16_t life_offset = TransTime(trail->life_offset);
	pack(life_offset, ptr);

	uint16_t fadeout_time = TransTime(trail->fadeout_time);
	pack(fadeout_time, ptr);

	if (mode == T2D_MODE_IMAGE) {
		uint16_t sz = trail->comp_images.size();
		pack(sz, ptr);
		for (int i = 0; i < sz; ++i) {
			PackCompImage(trail->comp_images[i], ptr);
		}
	} else {
		assert(mode == T2D_MODE_SHAPE);
		uint16_t sz = trail->comp_shapes.size();
		pack(sz, ptr);
		for (int i = 0; i < sz; ++i) {
			PackCompShape(trail->comp_shapes[i], ptr);
		}
	}
}

int TrailToBin::CompImageSize()
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint16_t) * 2;		// scale
	sz += sizeof(uint32_t) * 4;		// color
	return sz;
}

void TrailToBin::PackCompImage(const PackTrail::CompImage& comp, uint8_t** ptr)
{
	uint16_t id = comp.node->GetSprID();
	pack(id, ptr);

	uint16_t scale_begin = TransFloatX100(comp.scale_begin);
	pack(scale_begin, ptr);
	uint16_t scale_end = TransFloatX100(comp.scale_end);
	pack(scale_end, ptr);

	uint32_t mul_col_begin = comp.mul_col_begin;
	pack(mul_col_begin, ptr);
	uint32_t mul_col_end = comp.mul_col_end;
	pack(mul_col_end, ptr);
	uint32_t add_col_begin = comp.add_col_begin;
	pack(add_col_begin, ptr);
	uint32_t add_col_end = comp.add_col_end;
	pack(add_col_end, ptr);
}

int TrailToBin::CompShapeSize()
{
	int sz = 0;
	sz += sizeof(uint16_t);			// linewidth
	sz += sizeof(uint16_t);			// acuity
	sz += sizeof(uint32_t) * 2;		// color
	return sz;
}

void TrailToBin::PackCompShape(const PackTrail::CompShape& comp, uint8_t** ptr)
{
	uint16_t linewidth = static_cast<int>(comp.linewidth);
	pack(linewidth, ptr);

	uint16_t acuity = TransFloatX100(comp.acuity);
	pack(acuity, ptr);

	uint32_t col_begin = comp.col_begin;
	pack(col_begin, ptr);
	uint32_t col_end = comp.col_end;
	pack(col_end, ptr);
}

}