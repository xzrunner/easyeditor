#ifndef _EASYPARTICLE3D_PS_CONFIG_H_
#define _EASYPARTICLE3D_PS_CONFIG_H_

namespace eparticle3d
{

enum PS_PROPERTY
{
	PS_COUNT = 0,
	PS_EMISSION_TIME,
	PS_LIFE_TIME,
	PS_RADIAL_SPEED,
	PS_TANGENTIAL_SPEED,
	PS_ANGULAR_SPEED,
	PS_DISTURBANCE_RADIUS,
	PS_DISTURBANCE_SPD,
	PS_GRAVITY,
	PS_LINEAR_ACC,
	PS_FADEOUT_TIME,
	PS_START_POS,

	PS_SCALE,
	PS_ROTATE,
};

static const int MAX_COMPONENTS = 256;

}

#endif // _EASYPARTICLE3D_PS_CONFIG_H_