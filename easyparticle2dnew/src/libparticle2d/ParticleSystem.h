#ifndef _EASYPARTICLE2D_PARTICLE_SYSTEM_H_
#define _EASYPARTICLE2D_PARTICLE_SYSTEM_H_

#include <drag2d.h>

#include "UICallback.h"
#include "ValueRegion.h"

namespace eparticle2d
{

class Particle;

enum PS_PROPERTY
{
	PS_COUNT = 0,
	PS_EMISSION_TIME,
	PS_FADEOUT_TIME,

	PS_LIFE,
	PS_POSITION,
	PS_DIRECTION,
	PS_SCALE,
	PS_SPEED,
	PS_GRAVITY,
	PS_RADIAL_ACC,
	PS_TANGENTIAL_ACC,

	PS_COS_AMPLITUDE,
	PS_COS_FREQUENCY,

}; // PS_PROPERTY

class ParticleSystem : public d2d::Object, public UICallback
{
public:
	ParticleSystem(int buffer);
	~ParticleSystem();

	//
	// interface UICallback
	//
	virtual void OnSetKeyValue(int key, int val);
	virtual void OnSetKeyValue(int key, int val0, int val1);

	void Draw(const d2d::Matrix& mt);

	void Update(float dt);

	void Start();
	void Stop();
	void Reset();
	void Pause();

	void ReloadTexture() const;

	void SetSymbol(d2d::ISymbol* symbol);
	const wxString& GetSymbolFilePath() const {
		return m_symbol->getFilepath();
	}

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

	float m_scale_start, m_scale_end;

	d2d::ISymbol* m_symbol;

	//
	// For particle
	//

	ValueRegion<float> m_life_region;

	ValueRegion<float> m_posx_region, m_posy_region;
	ValueRegion<float> m_direction_region;
//	ValueRegion<float> m_scale_region;

	ValueRegion<float> m_speed_region;
	ValueRegion<float> m_gravity_region;
	ValueRegion<float> m_radial_acc_region;
	ValueRegion<float> m_tangential_acc_region;

	// special
	ValueRegion<float> m_cos_amplitude_region;
	ValueRegion<float> m_cos_frequency_region;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_PARTICLE_SYSTEM_H_