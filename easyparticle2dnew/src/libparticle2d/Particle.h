#ifndef _EASYPARTICLE2D_PARTICLE_H_
#define _EASYPARTICLE2D_PARTICLE_H_

#include <drag2d.h>

namespace eparticle2d
{

class Particle
{
public:
	// The lifetime of the particle emitter (-1 means infinite) and the life it has left.
	float lifetime;
	float life;

	// position
	d2d::Vector position;
	float direction;
	float scale;

 	// Particles gravitate towards this point.
 	d2d::Vector origin;

	// speed
	d2d::Vector speed;
	float gravity;
	float radial_acceleration;
	float tangential_acceleration;

	// ����
	float rotation;
// 	float spin_start;
// 	float spin_end;

	d2d::Colorf color;

}; // Patricle

}

#endif // _EASYPARTICLE2D_PARTICLE_H_