#include "ParticleSystem.h"
#include "ps_config.h"
#include "PS.h"

#include <ps_2d.h>

namespace eparticle2d
{

ParticleSystem::ParticleSystem(p2d_emitter_cfg* cfg)
{
	m_et = p2d_emitter_create(cfg);

	PS::Instance();
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
{
	m_pos = ps.m_pos;
	m_et = p2d_emitter_create(ps.m_et->cfg);

	PS::Instance();
}

ParticleSystem::~ParticleSystem()
{
	free(m_et);
}

void ParticleSystem::SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		m_et->cfg->count = data.val0;
		break;
	case PS_EMISSION_TIME:
		m_et->cfg->emission_time = data.val0 * 0.001f;
		break;
	case PS_LIFE_TIME:
		m_et->cfg->life = data.val0 * 0.001f;
		m_et->cfg->life_var = data.val1 * 0.001f;
		break;
	case PS_POSITION_X:
		m_et->cfg->position.x = data.val0;
		m_et->cfg->position_var.x = data.val1;
		break;
	case PS_POSITION_Y:
		m_et->cfg->position.y = data.val0;
		m_et->cfg->position_var.y = data.val1;
		break;
	case PS_DIRECTION:
		m_et->cfg->direction = data.val0 * d2d::TRANS_DEG_TO_RAD;
		m_et->cfg->direction_var = data.val1 * d2d::TRANS_DEG_TO_RAD;
		break;

	case PS_GRAVITY:
		m_et->cfg->mode.A.gravity.x = data.val0;
		m_et->cfg->mode.A.gravity.y = data.val1;
		break;
	case PS_SPEED:
		m_et->cfg->mode.A.speed = data.val0;
		m_et->cfg->mode.A.speed_var = data.val1;
		break;
	case PS_TANGENTIAL_ACCEL:
		m_et->cfg->mode.A.tangential_accel = data.val0;
		m_et->cfg->mode.A.tangential_accel_var = data.val1;
		break;
	case PS_RADIAL_ACCEL:
		m_et->cfg->mode.A.radial_accel = data.val0;
		m_et->cfg->mode.A.radial_accel_var = data.val1;
		break;

	case PS_START_RADIUS:
		m_et->cfg->mode.B.start_radius = data.val0;
		m_et->cfg->mode.B.start_radius_var = data.val1;
		break;
	case PS_END_RADIUS:
		m_et->cfg->mode.B.end_radius = data.val0;
		m_et->cfg->mode.B.end_radius_var = data.val1;
		break;
	case PS_DIRECTION_SPEED:
		m_et->cfg->mode.B.direction_delta = data.val0;
		m_et->cfg->mode.B.direction_delta_var = data.val1;
		break;

	case PS_COS_AMPLITUDE:
		m_et->cfg->mode.C.cos_amplitude = data.val0;
		m_et->cfg->mode.C.cos_amplitude_var = data.val1;
		break;
	case PS_COS_FREQUENCY:
		m_et->cfg->mode.C.cos_frequency = data.val0;
		m_et->cfg->mode.C.cos_frequency_var = data.val1;
		break;	
	}
}

void ParticleSystem::GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		data.val0 = m_et->cfg->count;
		break;
	case PS_EMISSION_TIME:
		data.val0 = m_et->cfg->emission_time * 1000;
		break;
	case PS_LIFE_TIME:
		data.val0 = m_et->cfg->life * 1000;
		data.val1 = m_et->cfg->life_var * 1000;
		break;
	case PS_POSITION_X:
		data.val0 = m_et->cfg->position.x;
		data.val1 = m_et->cfg->position_var.x;
		break;
	case PS_POSITION_Y:
		m_et->cfg->position.y = data.val0;
		m_et->cfg->position_var.y = data.val1;
		break;
	case PS_DIRECTION:
		data.val0 = m_et->cfg->direction * d2d::TRANS_RAD_TO_DEG;
		data.val1 = m_et->cfg->direction_var * d2d::TRANS_RAD_TO_DEG;
		break;

	case PS_GRAVITY:
		data.val0 = m_et->cfg->mode.A.gravity.x;
		data.val1 = m_et->cfg->mode.A.gravity.y;
		break;
	case PS_SPEED:
		data.val0 = m_et->cfg->mode.A.speed;
		data.val1 = m_et->cfg->mode.A.speed_var;
		break;
	case PS_TANGENTIAL_ACCEL:
		data.val0 = m_et->cfg->mode.A.tangential_accel;
		data.val1 = m_et->cfg->mode.A.tangential_accel_var;
		break;
	case PS_RADIAL_ACCEL:
		data.val0 = m_et->cfg->mode.A.radial_accel;
		data.val1 = m_et->cfg->mode.A.radial_accel_var;
		break;

	case PS_START_RADIUS:
		data.val0 = m_et->cfg->mode.B.start_radius;
		data.val1 = m_et->cfg->mode.B.start_radius_var;
		break;
	case PS_END_RADIUS:
		data.val0 = m_et->cfg->mode.B.end_radius;
		data.val1 = m_et->cfg->mode.B.end_radius_var;
		break;
	case PS_DIRECTION_SPEED:
		data.val0 = m_et->cfg->mode.B.direction_delta;
		data.val1 = m_et->cfg->mode.B.direction_delta_var;
		break;

	case PS_COS_AMPLITUDE:
		data.val0 = m_et->cfg->mode.C.cos_amplitude;
		data.val1 = m_et->cfg->mode.C.cos_amplitude_var;
		break;
	case PS_COS_FREQUENCY:
		data.val0 = m_et->cfg->mode.C.cos_frequency;
		data.val1 = m_et->cfg->mode.C.cos_frequency_var;
		break;	
	}
}

void ParticleSystem::Draw(const d2d::Matrix& mt) const
{
	p2d_emitter_draw(m_et, &mt);
}

void ParticleSystem::Update(float dt)
{
	p2d_emitter_update(m_et, dt);
}

void ParticleSystem::Start()
{
	m_et->active = true;
}

void ParticleSystem::Stop()
{
	m_et->active = false;
	m_et->emit_counter = 0;
}

void ParticleSystem::Reset()
{
	Clear();
}

void ParticleSystem::Pause()
{
	m_et->active = false;
}

void ParticleSystem::SetLoop(bool loop)
{
	if (loop == m_et->loop) {
		return;
	}

	m_et->loop = loop;

	if (m_et->loop) {
		Start();
	} else {
		Pause();
	}
}

void ParticleSystem::SetMode(int mode)
{
	m_et->cfg->mode_type = mode;
}

void ParticleSystem::SetRotationIsDir(bool is_dir)
{
	m_et->cfg->mode.A.rotation_is_dir = is_dir;
}

void ParticleSystem::Clear()
{
	p2d_emitter_clear(m_et);
}

bool ParticleSystem::IsEmpty() const
{
	return !m_et->head;
}

void ParticleSystem::ReloadTexture() const
{
	for (int i = 0; i < m_et->cfg->symbol_count; ++i) {
		d2d::ISymbol* symbol = static_cast<d2d::ISymbol*>(m_et->cfg->symbols[i].ud);
		symbol->ReloadTexture();
	}
}

p2d_symbol* ParticleSystem::AddSymbol(d2d::ISymbol* symbol)
{
	assert(m_et->cfg->symbol_count < MAX_COMPONENTS);

	p2d_symbol& comp = m_et->cfg->symbols[m_et->cfg->symbol_count++];
	memset(&comp, 0, SIZEOF_P2D_SYMBOL);

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
	if (idx < 0 || idx >= m_et->cfg->symbol_count) {
		return;
	}

	if (m_et->cfg->symbol_count == 1) {
		m_et->cfg->symbol_count = 0;
	} else {
		const p2d_symbol& src = m_et->cfg->symbols[--m_et->cfg->symbol_count];
		p2d_symbol& dst = m_et->cfg->symbols[idx];
		memcpy(&dst, &src, SIZEOF_P2D_SYMBOL);
	}
}

void ParticleSystem::DelAllSymbol()
{
	m_et->cfg->symbol_count = 0;
}

const p2d_emitter_cfg* ParticleSystem::GetConfig() const
{
	return m_et->cfg;
}

}