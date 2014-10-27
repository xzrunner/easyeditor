#include "ParticleSystem.h"

#include "Particle.h"

namespace eparticle2d
{

ParticleSystem::ParticleSystem(int buffer)
{
	m_active = false;

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

void ParticleSystem::OnSetKeyValue(int key, int val)
{
	switch (key)
	{
	case PS_COUNT:
		m_count = val;
		break;
	case PS_EMISSION_TIME:
		m_emission_time = val * 0.001f;
		m_lifetime = m_emission_time;
		break;
	case PS_FADEOUT_TIME:
		m_fadeout_time = val * 0.001f; 
		break;
	}	
}

void ParticleSystem::OnSetKeyValue(int key, int val0, int val1)
{
	switch (key)
	{
	case PS_LIFE:
		m_life_region.Set((val0 - val1) * 0.001f, (val0 + val1) * 0.001f);
		break;
	case PS_DIRECTION:
		m_direction_region.Set((val0 - val1) * d2d::TRANS_DEG_TO_RAD, (val0 + val1) * d2d::TRANS_DEG_TO_RAD);
		break;
	case PS_SCALE:
		m_scale_start = val0 * 0.01f;
		m_scale_end = val1 * 0.01f;
		break;
	case PS_SPEED:
		m_speed_region.Set(val0 - val1, val0 + val1);
		break;
	case PS_GRAVITY:
		m_gravity_region.Set(val0 - val1, val0 + val1);
		break;
	case PS_RADIAL_ACC:
		m_radial_acc_region.Set(val0 - val1, val0 + val1);
		break;
	case PS_TANGENTIAL_ACC:
		m_tangential_acc_region.Set(val0 - val1, val0 + val1);
		break;
	case PS_POSITION:
		m_posx_region.Set(-val0, val0);
		m_posy_region.Set(-val1, val1);
		break;
	case PS_COS_AMPLITUDE:
		m_cos_amplitude_region.Set(val0 - val1, val0 + val1);
		break;
	case PS_COS_FREQUENCY:
		m_cos_frequency_region.Set((val0 - val1) * 0.01f, (val0 + val1) * 0.01f);
		break;
	}
}

void ParticleSystem::Draw(const d2d::Matrix& mt)
{
	if (!m_symbol) {
		return;
	}

	Particle* p = m_start;
	while (p != m_last) 
	{
		d2d::SpriteDraw::drawSprite(m_symbol, mt, p->position, 
			p->rotation, p->scale, p->scale, 0, 0, p->color);
		++p;
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
		if (p->life > 0)
		{
			p->life -= dt;

			p->scale = (p->life / p->lifetime) * (m_scale_start - m_scale_end) + m_scale_end;

			if (p->life < m_fadeout_time) {
				p->color.a = p->life / m_fadeout_time;
			}

			d2d::Vector radial = p->position - p->origin;
			radial.normalize();
			radial *= p->radial_acceleration;

			d2d::Vector tangential = d2d::Math::rotateVectorRightAngle(radial, true);
			tangential *= p->tangential_acceleration;

			d2d::Vector gravity(0.0f, p->gravity);

			p->speed += (radial + tangential + gravity) * dt;

			p->speed.x = p->cos_amplitude * cos(p->cos_frequency * p->life);

			p->position += p->speed * dt;

			++p;
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

void ParticleSystem::Add()
{
	if (IsFull()) {
		return;
	}

	Particle* p = m_last;

	p->lifetime = m_life_region.Random();
	p->life = p->lifetime;

	p->position.x = m_posx_region.Random();
	p->position.y = m_posy_region.Random();
	p->origin = p->position;
	p->direction = m_direction_region.Random();

	float speed = m_speed_region.Random();
	p->speed = d2d::Vector(cos(p->direction), sin(p->direction)) * speed;
	p->gravity = m_gravity_region.Random();
	p->radial_acceleration = m_radial_acc_region.Random();
	p->tangential_acceleration = m_tangential_acc_region.Random();

	p->cos_amplitude = m_cos_amplitude_region.Random();
	p->cos_frequency = m_cos_frequency_region.Random();

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