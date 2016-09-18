#include "NodeParticle3d.h"
#include "ImportStream.h"
#include "Allocator.h"

namespace simp
{

NodeParticle3d::NodeParticle3d(Allocator& alloc, ImportStream& is)
{
	n = is.UInt16();
	alloc.Alloc(ComponentSize() * n);
	for (int i = 0; i < n; ++i)
	{
		Component* comp = &components[i];
		comp->sym_id		= is.UInt32();
		comp->count			= is.UInt16();
		comp->scale_start	= is.UInt16();
		comp->scale_end		= is.UInt16();
		comp->angle			= is.UInt16();
		comp->angle_var		= is.UInt16();
		comp->mul_col_begin = is.UInt32();
		comp->mul_col_end	= is.UInt32();
		comp->add_col_begin = is.UInt32();
		comp->add_col_end	= is.UInt32();
	}

	static_mode			= is.UInt8();

	emission_time		= is.UInt16();
	count				= is.UInt16();

	life				= is.UInt16();
	life_var			= is.UInt16();

	hori				= is.UInt16();
	hori_var			= is.UInt16();
	vert				= is.UInt16();
	vert_var			= is.UInt16();

	radial_spd			= is.UInt16();
	radial_spd_var		= is.UInt16();
	tangential_spd		= is.UInt16();
	tangential_spd_var	= is.UInt16();
	angular_spd			= is.UInt16();
	angular_spd_var		= is.UInt16();

	dis_region			= is.UInt16();
	dis_region_var		= is.UInt16();
	dis_spd				= is.UInt16();
	dis_spd_var			= is.UInt16();

	gravity				= is.UInt16();

	linear_acc			= is.UInt16();
	linear_acc_var		= is.UInt16();

	fadeout_time		= is.UInt16();

	ground				= is.UInt8();

	start_radius		= is.UInt16();
	start_height		= is.UInt16();

	orient_to_movement	= is.UInt8();

	blend				= is.UInt8();
}

int NodeParticle3d::Size()
{
	return sizeof(NodeParticle3d) - sizeof(Component);
}

int NodeParticle3d::ComponentSize()
{
	return sizeof(Component);
}

}