#include "TrailFromBin.h"
#include "UnpackNodeFactory.h"
#include "pack_unpack.h"

#include <spritepack.h>
#include <mt_2d.h>

namespace erespacker
{

int TrailFromBin::Size(const PackTrail* trail)
{
	int sz = 0;
	sz += SIZEOF_T2D_EMITTER_CFG;
	if (trail->mode == 0) {
		sz += trail->comp_images.size() * SIZEOF_T2D_SYMBOL;
	} else {
		assert(trail->mode == 1);
		sz += trail->comp_shapes.size() * SIZEOF_T2D_SYMBOL;
	}
	return sz;
}

void TrailFromBin::Unpack(uint8_t** ptr, PackTrail* trail)
{
	uint8_t mode;
	unpack(mode, ptr);
	trail->mode = mode;

	uint16_t count;
	unpack(count, ptr);
	trail->count = count;

	uint16_t life_begin, life_offset;
	unpack(life_begin, ptr);
	unpack(life_offset, ptr);
	trail->life_begin = TransTime(life_begin);
	trail->life_offset = TransTime(life_offset);

	uint16_t fadeout_time;
	unpack(fadeout_time, ptr);
	trail->fadeout_time = TransTime(fadeout_time);

	uint16_t comp_count;
	unpack(comp_count, ptr);
	if (mode == 0) {
		UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

		trail->comp_images.push_back(PackTrail::CompImage());
		PackTrail::CompImage& comp = trail->comp_images[trail->comp_images.size() - 1];

		uint16_t id;
		unpack(id, ptr);
		comp.node = factory->Query(id);
		if (!comp.node) {
			factory->AddUnassigned(id, &comp.node);
		}

		uint16_t scale_begin, scale_end;
		unpack(scale_begin, ptr);
		unpack(scale_end, ptr);
		comp.scale_begin = TransFloatX100(scale_begin);
		comp.scale_end = TransFloatX100(scale_end);

		uint32_t mul_col_begin, mul_col_end, 
				 add_col_begin, add_col_end;
		unpack(mul_col_begin, ptr);
		unpack(mul_col_end, ptr);
		unpack(add_col_begin, ptr);
		unpack(add_col_end, ptr);
		comp.mul_col_begin = mul_col_begin;
		comp.mul_col_end = mul_col_end;
		comp.add_col_begin = add_col_begin;
		comp.add_col_end = add_col_end;		
	} else {
		PackTrail::CompShape comp;

		uint16_t linewidth;
		unpack(linewidth, ptr);
		comp.linewidth = linewidth;

		uint16_t acuity;
		unpack(acuity, ptr);
		comp.acuity = TransFloatX100(acuity);

		uint32_t col_begin, col_end;
		unpack(col_begin, ptr);
		unpack(col_end, ptr);
		comp.col_begin = col_begin;
		comp.col_end = col_end;

		trail->comp_shapes.push_back(comp);
	}
}

}