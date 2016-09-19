#include "NodeTrail.h"
#include "ImportStream.h"
#include "Allocator.h"

#include <mt_2d.h>

#include <assert.h>

namespace simp
{

NodeTrail::NodeTrail(Allocator& alloc, ImportStream& is)
{
	mode_type = is.UInt8();
	
	n = is.UInt16();
	alloc.Alloc(ComponentSize() * n);
	if (mode_type == T2D_MODE_IMAGE)
	{
		for (int i = 0; i < n; ++i)
		{
			Component* comp = &components[i];
			comp->mode.A.sym_id			= is.UInt32();
			comp->mode.A.scale_begin	= is.UInt16();
			comp->mode.A.scale_end		= is.UInt16();
			comp->col_begin				= is.UInt32();
			comp->col_end				= is.UInt32();
			comp->mode.A.add_col_begin	= is.UInt32();
			comp->mode.A.add_col_end	= is.UInt32();
		}
	}
	else
	{
		assert(mode_type == T2D_MODE_SHAPE);
		for (int i = 0; i < n; ++i)
		{
			Component* comp = &components[i];
			comp->mode.B.size			= is.UInt16();
			comp->mode.B.acuity			= is.UInt16();
			comp->col_begin				= is.UInt32();
			comp->col_end				= is.UInt32();
		}
	}

	count			= is.UInt16();
	
	life_begin		= is.UInt16();
	life_offset		= is.UInt16();

	fadeout_time	= is.UInt16();
}

int NodeTrail::Size()
{
	return sizeof(NodeTrail) - sizeof(Component);
}

int NodeTrail::ComponentSize()
{
	return sizeof(Component);
}

}