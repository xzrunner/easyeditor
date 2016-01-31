#ifndef _EASYP3DINV_PARTICLE_SYSTEM_H_
#define _EASYP3DINV_PARTICLE_SYSTEM_H_



#include "Particle.h"

namespace ep3dinv
{

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Update(float dt);

	void Draw(const ee::Matrix& mt) const;

	void Reset();

	void LoadFromFile(const std::string& filepath);

private:
	void Clear();

private:
	std::vector<Particle*> m_particles;

	float m_gravity;

	float m_during;

}; // ParticleSystem

}

#endif // _EASYP3DINV_PARTICLE_SYSTEM_H_