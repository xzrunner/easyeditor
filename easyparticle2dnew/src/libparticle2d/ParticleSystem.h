#ifndef _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE2D_PARTICLE_SYSTEM_H_

#include <drag2d.h>

#include "ValueRegion.h"

namespace eparticle2d
{

class Particle;

class ParticleSystem : public d2d::Object
{
public:
	ParticleSystem(int buffer);
	~ParticleSystem();

	void Draw(const d2d::Matrix& mt);

	void Update(float dt);

	void Start();
	void Stop();
	void Reset();
	void Pause();

	void ReloadTexture() const;

	void SetSymbol(d2d::ISymbol* symbol);

public:
	void SetCount(int val);
	void SetEmissionTime(int val);
	void SetFadeoutTime(int val);

	void SetLife(int min, int max);
	void SetPosition(const d2d::Vector& min, const d2d::Vector& max);
	void SetDirection(int min, int max);
	void SetScale(int min, int max);

	void SetSpeed(const d2d::Vector& min, const d2d::Vector& max);
	void SetGravity(int min, int max);
	void SetRadialAcc(int min, int max);
	void SetTangentialAcc(int min, int max);

private:
	void Add();
	void Remove(Particle * p);

	bool IsFull() const;
	bool IsEmpty() const;

private:
	bool m_active;

	// Pointer to the first particle.
	Particle* m_start;
	// Pointer to the next available free space.
	Particle* m_last;
	// Pointer to the end of the memory allocation.
	Particle* m_end;

	// Used to determine when a Particle should be emitted.
	float m_emit_counter;

	// 发射完count个粒子所需要的时间
	float m_emission_time;
	int m_count;

	// The lifetime of the particle emitter (-1 means infinite) and the life it has left.
	float m_lifetime;
	float m_life;

	float m_fadeout_time;

	d2d::ISymbol* m_symbol;

	//
	// For particle
	//

	ValueRegion<float> m_life_region;

	ValueRegion<d2d::Vector> m_pos_region;
	ValueRegion<float> m_direction_region;
	ValueRegion<float> m_scale_region;

	ValueRegion<d2d::Vector> m_speed_region;
	ValueRegion<float> m_gravity_region;
	ValueRegion<float> m_radial_acc_region;
	ValueRegion<float> m_tangential_acc_region;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_PARTICLE_SYSTEM_H_