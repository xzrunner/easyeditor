#include "ParticleSystem.h"
#include "Particle.h"
#include "config.h"
#include "ps_config.h"

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

	m_lifetime = DURATION_INFINITY;
	m_life = 0;
	m_fadeout_time = 0;

	m_symbol = NULL;
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_start;

	if (m_symbol) {
		m_symbol->Release();
	}
}

void ParticleSystem::SetValue(int key, const UICallback::Data& data)
{
	switch (key)
	{
	case PS_DURATION:
		m_lifetime = data.val0;
		break;
	case PS_COUNT:
		m_count = data.val0;
		break;
	case PS_EMISSION_TIME:
		m_emission_time = data.val0;
		break;
	case PS_FADEOUT_TIME:
		m_fadeout_time = data.val0; 
		break;

	case PS_LIFE:
		m_life_region.Set(data.val0 - data.val1, data.val0 + data.val1);
		break;
	case PS_DIRECTION:
		m_direction_region.Set((data.val0 - data.val1) * d2d::TRANS_DEG_TO_RAD, (data.val0 + data.val1) * d2d::TRANS_DEG_TO_RAD);
		break;
	case PS_SCALE:
		m_scale_start = data.val0;
		m_scale_end = data.val1;
		break;
	case PS_SPEED:
		m_speed_region.Set(data.val0 - data.val1, data.val0 + data.val1);
		break;
	case PS_GRAVITY:
		m_gravity_region.Set(data.val0 - data.val1, data.val0 + data.val1);
		break;
	case PS_RADIAL_ACC:
		m_radial_acc_region.Set(data.val0 - data.val1, data.val0 + data.val1);
		break;
	case PS_TANGENTIAL_ACC:
		m_tangential_acc_region.Set(data.val0 - data.val1, data.val0 + data.val1);
		break;
	case PS_POSITION:
		m_posx_region.Set(-data.val0, data.val0);
		m_posy_region.Set(-data.val1, data.val1);
		break;
	case PS_COS_AMPLITUDE:
		m_cos_amplitude_region.Set(data.val0 - data.val1, data.val0 + data.val1);
		break;
	case PS_COS_FREQUENCY:
		m_cos_frequency_region.Set(data.val0 - data.val1, data.val0 + data.val1);
		break;
	}
}

void ParticleSystem::GetValue(int key, UICallback::Data& data)
{
	switch (key)
	{
	case PS_DURATION:
		data.val0 = m_lifetime;
		break;
	case PS_COUNT:
		data.val0 = m_count;
		break;
	case PS_EMISSION_TIME:
		data.val0 = m_emission_time;
		break;
	case PS_FADEOUT_TIME:
		data.val0 = m_fadeout_time;
		break;

	case PS_LIFE:
		{
			float sub = m_life_region.Begin(),
				add = m_life_region.End();
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
		break;
	case PS_DIRECTION:
		{
			float sub = m_direction_region.Begin() * d2d::TRANS_RAD_TO_DEG,
				add = m_direction_region.End() * d2d::TRANS_RAD_TO_DEG;
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
		break;
	case PS_SCALE:
		data.val0 = m_scale_start;
		data.val1 = m_scale_end;
		break;
	case PS_SPEED:
		{
			float sub = m_speed_region.Begin(),
				add = m_speed_region.End();
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
		break;
	case PS_GRAVITY:
		{
			float sub = m_gravity_region.Begin(),
				add = m_gravity_region.End();
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
		break;
	case PS_RADIAL_ACC:
		{
			float sub = m_radial_acc_region.Begin(),
				add = m_radial_acc_region.End();
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
		break;
	case PS_TANGENTIAL_ACC:
		{
			float sub = m_tangential_acc_region.Begin(),
				add = m_tangential_acc_region.End();
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
		break;
	case PS_POSITION:
		data.val0 = m_posx_region.End();
		data.val1 = m_posy_region.End();
		break;
	case PS_COS_AMPLITUDE:
		{
			float sub = m_cos_amplitude_region.Begin(),
				add = m_cos_amplitude_region.End();
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
		break;
	case PS_COS_FREQUENCY:
		{
			float sub = m_cos_frequency_region.Begin(),
				add = m_cos_frequency_region.End();
			data.val0 = (sub + add) * 0.5f;
			data.val1 = (add - sub) * 0.5f;
		}
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
		if (m_lifetime != DURATION_INFINITY && m_life < 0) {
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
			d2d::Vector tangential = d2d::Math::rotateVectorRightAngle(radial, true);

			radial *= p->radial_acceleration;			
			tangential *= p->tangential_acceleration;

			d2d::Vector gravity(0.0f, -p->gravity);

			p->speed += (radial + tangential + gravity) * dt;

			// 水平速度cos效果器
			if (p->cos_amplitude != 0) {
				p->speed.x = p->cos_amplitude * cos(p->cos_frequency * (p->lifetime - p->life));
			}

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
	m_life = m_lifetime;
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
			m_symbol->Release();
		}
		m_symbol = symbol;
		if (m_symbol) {
			m_symbol->Retain();
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