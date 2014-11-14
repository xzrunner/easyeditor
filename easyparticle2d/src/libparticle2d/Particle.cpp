#include "Particle.h"

namespace eparticle2d
{

Particle::Particle()
{
	lifetime = life = 0;

	direction = 0;
	scale = 1;

	gravity = 0;
	radial_acceleration = tangential_acceleration = 0;

	rotation = 0;

	cos_amplitude = cos_frequency = 0;
}

}