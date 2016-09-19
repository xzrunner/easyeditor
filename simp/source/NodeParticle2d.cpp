#include "NodeParticle2d.h"
#include "ImportStream.h"
#include "Allocator.h"

#include <ps_2d.h>

#include <assert.h>

namespace simp
{

NodeParticle2d::NodeParticle2d(Allocator& alloc, ImportStream& is)
{
	n = is.UInt16();
	alloc.Alloc(ComponentSize() * n);
	for (int i = 0; i < n; ++i)
	{
		Component* comp = &components[i];
		comp->sym_id		= is.UInt32();
		comp->angle_start	= is.UInt16();
		comp->angle_end		= is.UInt16();
		comp->scale_start	= is.UInt16();
		comp->scale_end		= is.UInt16();
		comp->mul_col_begin = is.UInt32();
		comp->mul_col_end	= is.UInt32();
		comp->add_col_begin = is.UInt32();
		comp->add_col_end	= is.UInt32();
	}

	emission_time		= is.UInt16();
	count				= is.UInt16();
	
	life				= is.UInt16();
	life_var			= is.UInt16();

	position[0]			= is.UInt16();
	position[1]			= is.UInt16();
	position_var[0]		= is.UInt16();
	position_var[1]		= is.UInt16();

	direction			= is.UInt16();
	direction_var		= is.UInt16();

	mode_type			= is.UInt8();
	if (mode_type == P2D_MODE_GRAVITY) 
	{
		mode.A.gravity[0]			= is.UInt16();
		mode.A.gravity[1]			= is.UInt16();

		mode.A.speed				= is.UInt16();
		mode.A.speed_var			= is.UInt16();

		mode.A.tangential_accel		= is.UInt16();
		mode.A.tangential_accel_var	= is.UInt16();

		mode.A.radial_accel			= is.UInt16();
		mode.A.radial_accel_var		= is.UInt16();

		mode.A.rotation_is_dir		= is.UInt8();
	}
	else if (mode_type == P2D_MODE_RADIUS)
	{
		mode.B.start_radius			= is.UInt16();
		mode.B.start_radius_var		= is.UInt16();
		mode.B.end_radius			= is.UInt16();
		mode.B.end_radius_var		= is.UInt16();
		mode.B.direction_delta		= is.UInt16();
		mode.B.direction_delta_var	= is.UInt16();
	}
	else if (mode_type == P2D_MODE_SPD_COS)
	{
		mode.C.speed				= is.UInt16();
		mode.C.speed_var			= is.UInt16();
		mode.C.cos_amplitude		= is.UInt16();
		mode.C.cos_amplitude_var	= is.UInt16();
		mode.C.cos_frequency		= is.UInt16();
		mode.C.cos_frequency_var	= is.UInt16();
	}
	else
	{
		assert(0);
	}
}

int NodeParticle2d::Size()
{
	return sizeof(NodeParticle2d) - sizeof(Component);
}

int NodeParticle2d::ComponentSize()
{
	return sizeof(Component);
}

}