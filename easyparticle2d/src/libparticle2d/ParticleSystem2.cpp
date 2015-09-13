#include "ParticleSystem2.h"
#include "ps_config2.h"

#include <particle2d.h>

namespace eparticle2d
{

ParticleSystem::ParticleSystem(unsigned int buffer, p2d_ps_config* cfg)
{
	m_ps = p2d_create(buffer, cfg);
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
{
	m_pos = ps.m_pos;
	m_ps = p2d_create(PARTICLE_CAP, ps.m_ps->cfg);
}

ParticleSystem::~ParticleSystem()
{
	free(m_ps);
}

void ParticleSystem::SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		m_ps->cfg->count = data.val0;
		break;
	case PS_EMISSION_TIME:
		m_ps->cfg->emission_time = data.val0;
		break;
	case PS_LIFE_TIME:
		m_ps->cfg->life = data.val0;
		m_ps->cfg->life_var = data.val1;
		break;
	case PS_POSITION_X:
		m_ps->cfg->position.x = data.val0;
		m_ps->cfg->position_var.x = data.val1;
		break;
	case PS_POSITION_Y:
		m_ps->cfg->position.y = data.val0;
		m_ps->cfg->position_var.y = data.val1;
		break;
	case PS_DIRECTION:
		m_ps->cfg->direction = data.val0;
		m_ps->cfg->direction_var = data.val1;
		break;

	case PS_GRAVITY:
		m_ps->cfg->mode.A.gravity.x = data.val0;
		m_ps->cfg->mode.A.gravity.y = data.val1;
		break;
	case PS_SPEED:
		m_ps->cfg->mode.A.speed = data.val0;
		m_ps->cfg->mode.A.speed_var = data.val1;
		break;
	case PS_TANGENTIAL_ACCEL:
		m_ps->cfg->mode.A.tangential_accel = data.val0;
		m_ps->cfg->mode.A.tangential_accel_var = data.val1;
		break;
	case PS_RADIAL_ACCEL:
		m_ps->cfg->mode.A.radial_accel = data.val0;
		m_ps->cfg->mode.A.radial_accel_var = data.val1;
		break;

	case PS_START_RADIUS:
		m_ps->cfg->mode.B.start_radius = data.val0;
		m_ps->cfg->mode.B.start_radius_var = data.val1;
		break;
	case PS_END_RADIUS:
		m_ps->cfg->mode.B.end_radius = data.val0;
		m_ps->cfg->mode.B.end_radius_var = data.val1;
		break;
	case PS_DIRECTION_SPEED:
		m_ps->cfg->mode.B.direction_delta = data.val0;
		m_ps->cfg->mode.B.direction_delta_var = data.val1;
		break;

	case PS_COS_AMPLITUDE:
		m_ps->cfg->mode.C.cos_amplitude = data.val0;
		m_ps->cfg->mode.C.cos_amplitude_var = data.val1;
		break;
	case PS_COS_FREQUENCY:
		m_ps->cfg->mode.C.cos_frequency = data.val0;
		m_ps->cfg->mode.C.cos_frequency_var = data.val1;
		break;	
	}
}

void ParticleSystem::GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		data.val0 = m_ps->cfg->count;
		break;
	case PS_EMISSION_TIME:
		data.val0 = m_ps->cfg->emission_time;
		break;
	case PS_LIFE_TIME:
		data.val0 = m_ps->cfg->life;
		data.val1 = m_ps->cfg->life_var;
		break;
	case PS_POSITION_X:
		data.val0 = m_ps->cfg->position.x;
		data.val1 = m_ps->cfg->position_var.x;
		break;
	case PS_POSITION_Y:
		m_ps->cfg->position.y = data.val0;
		m_ps->cfg->position_var.y = data.val1;
		break;
	case PS_DIRECTION:
		data.val0 = m_ps->cfg->direction;
		data.val1 = m_ps->cfg->direction_var;
		break;

	case PS_GRAVITY:
		data.val0 = m_ps->cfg->mode.A.gravity.x;
		data.val1 = m_ps->cfg->mode.A.gravity.y;
		break;
	case PS_SPEED:
		data.val0 = m_ps->cfg->mode.A.speed;
		data.val1 = m_ps->cfg->mode.A.speed_var;
		break;
	case PS_TANGENTIAL_ACCEL:
		data.val0 = m_ps->cfg->mode.A.tangential_accel;
		data.val1 = m_ps->cfg->mode.A.tangential_accel_var;
		break;
	case PS_RADIAL_ACCEL:
		data.val0 = m_ps->cfg->mode.A.radial_accel;
		data.val1 = m_ps->cfg->mode.A.radial_accel_var;
		break;

	case PS_START_RADIUS:
		data.val0 = m_ps->cfg->mode.B.start_radius;
		data.val1 = m_ps->cfg->mode.B.start_radius_var;
		break;
	case PS_END_RADIUS:
		data.val0 = m_ps->cfg->mode.B.end_radius;
		data.val1 = m_ps->cfg->mode.B.end_radius_var;
		break;
	case PS_DIRECTION_SPEED:
		data.val0 = m_ps->cfg->mode.B.direction_delta;
		data.val1 = m_ps->cfg->mode.B.direction_delta_var;
		break;

	case PS_COS_AMPLITUDE:
		data.val0 = m_ps->cfg->mode.C.cos_amplitude;
		data.val1 = m_ps->cfg->mode.C.cos_amplitude_var;
		break;
	case PS_COS_FREQUENCY:
		data.val0 = m_ps->cfg->mode.C.cos_frequency;
		data.val1 = m_ps->cfg->mode.C.cos_frequency_var;
		break;	
	}
}

void ParticleSystem::Draw(const d2d::Matrix& mt) const
{
	p2d_particle* p = m_ps->start;
	while (p != m_ps->last)
	{
		d2d::Colorf mul_col, add_col;
		memcpy(&mul_col.r, &p->col_mul.r, sizeof(float) * 4);
		memcpy(&add_col.r, &p->col_add.r, sizeof(float) * 4);

		d2d::Vector pos;
		pos.x = p->position.x;
		pos.y = p->position.y;

		float s = p->scale;

		d2d::ISymbol* symbol = static_cast<d2d::ISymbol*>(p->symbol->ud);
		d2d::SpriteRenderer::Instance()->Draw(symbol, d2d::Matrix(), pos, p->angle, s, s, 0, 0, mul_col, add_col);

		++p;
	}
}

void ParticleSystem::Update(float dt)
{
	p2d_update(m_ps, dt);
}

void ParticleSystem::Start()
{
	m_ps->is_active = true;
}

void ParticleSystem::Stop()
{
	m_ps->is_active = false;
	m_ps->emit_counter = 0;
}

void ParticleSystem::Reset()
{
	m_ps->last = m_ps->start;
	m_ps->emit_counter = 0;
}

void ParticleSystem::Pause()
{
	m_ps->is_active = false;
}

void ParticleSystem::SetLoop(bool loop)
{
	if (loop == m_ps->is_loop) {
		return;
	}

	m_ps->is_loop = loop;

	if (m_ps->is_loop) {
		Start();
	} else {
		Pause();
	}
}

void ParticleSystem::Clear()
{
	m_ps->last = m_ps->start;
}

bool ParticleSystem::IsEmpty() const
{
	return m_ps->start == m_ps->last;
}

void ParticleSystem::ReloadTexture() const
{
	for (int i = 0; i < m_ps->cfg->symbol_count; ++i) {
		d2d::ISymbol* symbol = static_cast<d2d::ISymbol*>(m_ps->cfg->symbols[i].ud);
		symbol->ReloadTexture();
	}
}

p2d_symbol* ParticleSystem::AddSymbol(d2d::ISymbol* symbol)
{
	assert(m_ps->cfg->symbol_count < MAX_COMPONENTS);

	p2d_symbol& comp = m_ps->cfg->symbols[m_ps->cfg->symbol_count++];
	memset(&comp, 0, sizeof(p2d_symbol));

	comp.angle_start = comp.angle_end = 0;

	comp.scale_start = comp.scale_end = 0;

	comp.col_mul_start.r = comp.col_mul_start.g = comp.col_mul_start.b = comp.col_mul_start.a = 1;
	comp.col_mul_end = comp.col_mul_start;
	memset(&comp.col_add_start, 0, sizeof(comp.col_add_start));
	memset(&comp.col_add_end, 0, sizeof(comp.col_add_end));

	comp.ud = symbol;

	return &comp;
}

void ParticleSystem::DelSymbol(int idx)
{
	if (idx < 0 || idx >= m_ps->cfg->symbol_count) {
		return;
	}

	if (m_ps->cfg->symbol_count == 1) {
		m_ps->cfg->symbol_count = 0;
	} else {
		const p2d_symbol& src = m_ps->cfg->symbols[--m_ps->cfg->symbol_count];
		p2d_symbol& dst = m_ps->cfg->symbols[idx];
		memcpy(&dst, &src, sizeof(p2d_symbol));
	}
}

void ParticleSystem::DelAllSymbol()
{
	m_ps->cfg->symbol_count = 0;
}

const p2d_ps_config* ParticleSystem::GetConfig() const
{
	return m_ps->cfg;
}

}