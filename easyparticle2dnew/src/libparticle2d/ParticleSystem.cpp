#include "ParticleSystem.h"

#include "Particle.h"

namespace eparticle2d
{

ParticleSystem::ParticleSystem(int buffer)
{
	m_last = m_start = new Particle[buffer];
	m_end = m_start + buffer;

	m_emit_counter = 0;

	m_emission_time = 0;
	m_count = 0;

	m_lifetime = m_life = 0;
	m_fadeout_time = 0;

	m_symbol = NULL;
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_start;

	if (m_symbol) {
		m_symbol->release();
	}
}

void ParticleSystem::Draw(const d2d::Matrix& mt)
{
	Particle* p = m_start;
	while (p != m_last) 
	{
		if (p->life < m_fadeout_time) {
			p->color.a = p->life / m_fadeout_time;
		}
		d2d::SpriteDraw::drawSprite(m_symbol, mt, p->position, 
			p->rotation, p->scale, p->scale, 0, 0, p->color);
	}
}

void ParticleSystem::Update(float dt)
{
	if (m_active) 
	{
		float rate = m_emission_time / m_count;
		m_emit_counter += dt;
		while (m_emit_counter > rate)
		{
			Add();
			m_emit_counter -= rate;
		}

		m_life -= dt;
		if (m_lifetime != -1 && m_life < 0) {
			Stop();
		}
	}

	Particle* p = m_start;
	while (p != m_last)
	{
		p->life -= dt;
		if (p->life > 0)
		{
			d2d::Vector radial = p->position - p->origin;
			radial.normalize();
			radial *= p->radial_acceleration;

			d2d::Vector tangential = d2d::Math::rotateVectorRightAngle(radial, true);
			tangential *= p->tangential_acceleration;

			d2d::Vector gravity(0.0f, p->gravity);

			p->speed += (radial + tangential + gravity) * dt;

			p->position += p->speed * dt;
		}
		else
		{
			Remove(p);
			if (p > m_last) {
				return;
			}
		}
	}
}

void ParticleSystem::Start()
{
	m_active = true;
}

void ParticleSystem::Stop()
{
	m_active = false;
	m_life = m_lifetime;
	m_emit_counter = 0;
}

void ParticleSystem::Reset()
{
	m_last = m_start;
	m_life = m_lifetime = m_emission_time;
	m_emit_counter = 0;
}

void ParticleSystem::Pause()
{
	m_active = false;
}

void ParticleSystem::ReloadTexture() const
{
	if (m_symbol) {
		m_symbol->reloadTexture();
	}
}

void ParticleSystem::SetSymbol(d2d::ISymbol* symbol)
{
	if (m_symbol != symbol)
	{
		if (m_symbol) {
			m_symbol->release();
		}
		m_symbol = symbol;
		if (m_symbol) {
			m_symbol->retain();
		}
	}
}

void ParticleSystem::SetCount(int val) 
{ 
	m_count = val; 
}

void ParticleSystem::SetEmissionTime(int val) 
{ 
	m_emission_time = val * 0.001f;
	m_lifetime = m_emission_time;
}

void ParticleSystem::SetFadeoutTime(int val) 
{ 
	m_fadeout_time = val * 0.001f; 
}

void ParticleSystem::SetLife(int min, int max) 
{ 
	m_life_region.Set(min, max); 
}

void ParticleSystem::SetPosition(const d2d::Vector& min, const d2d::Vector& max) 
{ 
	m_pos_region.Set(min, max); 
}

void ParticleSystem::SetDirection(int min, int max) 
{ 
	m_direction_region.Set(min, max);
}

void ParticleSystem::SetScale(int min, int max)
{
	m_scale_region.Set(min, max);
}

void ParticleSystem::SetSpeed(const d2d::Vector& min, const d2d::Vector& max)
{
	m_speed_region.Set(min, max);
}

void ParticleSystem::SetGravity(int min, int max)
{
	m_gravity_region.Set(min, max);
}

void ParticleSystem::SetRadialAcc(int min, int max)
{
	m_radial_acc_region.Set(min, max);
}

void ParticleSystem::SetTangentialAcc(int min, int max)
{
	m_tangential_acc_region.Set(min, max);
}

void ParticleSystem::Add()
{
	if (IsFull()) {
		return;
	}

	Particle* p = m_last;

	p->lifetime = m_life_region.Random();
	p->life = p->lifetime;

	p->position = m_pos_region.Random();
	p->origin = p->position;
	p->direction = m_direction_region.Random();
	p->scale = m_scale_region.Random();

	p->speed = m_speed_region.Random();
	p->gravity = m_gravity_region.Random();
	p->radial_acceleration = m_radial_acc_region.Random();
	p->tangential_acceleration = m_tangential_acc_region.Random();

	p->rotation = 0;

	p->color = d2d::WHITE;

	m_last++;
}

void ParticleSystem::Remove(Particle * p)
{
	if (!IsEmpty()) {
		*p = *(--m_last);
	}
}

bool ParticleSystem::IsFull() const
{
	return m_last == m_end;
}

bool ParticleSystem::IsEmpty() const
{
	return m_start == m_last;
}

}