#include "ParticleSystem.h"
#include "ps_config.h"

#include <ee/Math2D.h>
#include <ee/Symbol.h>

#include <ps_2d.h>
#include <sprite2/Particle2d.h>

#include <assert.h>

namespace eparticle2d
{

ParticleSystem::ParticleSystem(p2d_emitter_cfg* cfg)
{
	s2::Particle2d::Instance();

	m_et = p2d_emitter_create(cfg);
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
{
	s2::Particle2d::Instance();

	m_pos = ps.m_pos;
	m_et = p2d_emitter_create(ps.m_et->cfg);
}

ParticleSystem::~ParticleSystem()
{
	free(m_et);
}

void ParticleSystem::SetValue(int key, const ee::UICallback::Data& data)
{
	p2d_emitter_cfg* cfg = const_cast<p2d_emitter_cfg*>(m_et->cfg);
	switch (key)
	{
	case PS_COUNT:
		cfg->count = static_cast<int>(data.val0);
		break;
	case PS_EMISSION_TIME:
		cfg->emission_time = data.val0 * 0.001f;
		break;
	case PS_LIFE_TIME:
		cfg->life = data.val0 * 0.001f;
		cfg->life_var = data.val1 * 0.001f;
		break;
	case PS_POSITION_X:
		cfg->position.x = data.val0;
		cfg->position_var.x = data.val1;
		break;
	case PS_POSITION_Y:
		cfg->position.y = data.val0;
		cfg->position_var.y = data.val1;
		break;
	case PS_DIRECTION:
		cfg->direction = data.val0 * SM_DEG_TO_RAD;
		cfg->direction_var = data.val1 * SM_DEG_TO_RAD;
		break;

	case PS_GRAVITY:
		cfg->mode.A.gravity.x = data.val0;
		cfg->mode.A.gravity.y = data.val1;
		break;
	case PS_SPEED:
		cfg->mode.A.speed = data.val0;
		cfg->mode.A.speed_var = data.val1;
		break;
	case PS_TANGENTIAL_ACCEL:
		cfg->mode.A.tangential_accel = data.val0;
		cfg->mode.A.tangential_accel_var = data.val1;
		break;
	case PS_RADIAL_ACCEL:
		cfg->mode.A.radial_accel = data.val0;
		cfg->mode.A.radial_accel_var = data.val1;
		break;

	case PS_START_RADIUS:
		cfg->mode.B.start_radius = data.val0;
		cfg->mode.B.start_radius_var = data.val1;
		break;
	case PS_END_RADIUS:
		cfg->mode.B.end_radius = data.val0;
		cfg->mode.B.end_radius_var = data.val1;
		break;
	case PS_DIRECTION_SPEED:
		cfg->mode.B.direction_delta = data.val0;
		cfg->mode.B.direction_delta_var = data.val1;
		break;

	case PS_COS_AMPLITUDE:
		cfg->mode.C.cos_amplitude = data.val0;
		cfg->mode.C.cos_amplitude_var = data.val1;
		break;
	case PS_COS_FREQUENCY:
		cfg->mode.C.cos_frequency = data.val0;
		cfg->mode.C.cos_frequency_var = data.val1;
		break;	
	}
}

void ParticleSystem::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		data.val0 = static_cast<float>(m_et->cfg->count);
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
		const_cast<p2d_emitter_cfg*>(m_et->cfg)->position.y = data.val0;
		const_cast<p2d_emitter_cfg*>(m_et->cfg)->position_var.y = data.val1;
		break;
	case PS_DIRECTION:
		data.val0 = m_et->cfg->direction * SM_RAD_TO_DEG;
		data.val1 = m_et->cfg->direction_var * SM_RAD_TO_DEG;
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

void ParticleSystem::Draw(const S2_MAT& mt) const
{
	if (m_et->local_mode_draw) {
		p2d_emitter_draw(m_et, &mt);
	} else {
		p2d_emitter_draw(m_et, NULL);
	}
}

bool ParticleSystem::Update(const S2_MAT& mat)
{
	float time = s2::Particle2d::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	float mt[6];
	mt[0] = mat.x[0];
	mt[1] = mat.x[1];
	mt[2] = mat.x[4];
	mt[3] = mat.x[5];
	mt[4] = mat.x[12];
	mt[5] = mat.x[13];	

	float dt = time - m_et->time;
	p2d_emitter_update(m_et, dt, mt);
	m_et->time = time;

	return true;
}

void ParticleSystem::Start()
{
	p2d_emitter_start(m_et);
}

void ParticleSystem::Stop()
{
	p2d_emitter_stop(m_et);
}

void ParticleSystem::Reset()
{
	Clear();
}

void ParticleSystem::Pause()
{
	p2d_emitter_pause(m_et);
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

void ParticleSystem::SetLocalModeDraw(bool local)
{
	m_et->local_mode_draw = local;
}

void ParticleSystem::SetMode(int mode)
{
	const_cast<p2d_emitter_cfg*>(m_et->cfg)->mode_type = mode;
}

void ParticleSystem::SetRotationIsDir(bool is_dir)
{
	const_cast<p2d_emitter_cfg*>(m_et->cfg)->mode.A.rotation_is_dir = is_dir;
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
	for (int i = 0; i < m_et->cfg->sym_count; ++i) {
		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(m_et->cfg->syms[i].ud));
		sym->ReloadTexture();
	}
}

p2d_symbol* ParticleSystem::AddSymbol(s2::Symbol* sym)
{
	assert(m_et->cfg->sym_count < MAX_COMPONENTS);

	p2d_symbol& comp = m_et->cfg->syms[const_cast<p2d_emitter_cfg*>(m_et->cfg)->sym_count++];
	memset(&comp, 0, SIZEOF_P2D_SYMBOL);

	comp.angle_start = comp.angle_end = 0;

	comp.scale_start = comp.scale_end = 0;

	comp.mul_col_begin.r = comp.mul_col_begin.g = comp.mul_col_begin.b = comp.mul_col_begin.a = 255;
	comp.mul_col_end.r = comp.mul_col_end.g = comp.mul_col_end.b = comp.mul_col_end.a = 255;
	comp.add_col_begin.r = comp.add_col_begin.g = comp.add_col_begin.b = comp.add_col_begin.a = 0;
	comp.add_col_end.r = comp.add_col_end.g = comp.add_col_end.b = comp.add_col_end.a = 0;

	comp.ud = sym;

	return &comp;
}

void ParticleSystem::DelSymbol(int idx)
{
	if (idx < 0 || idx >= m_et->cfg->sym_count) {
		return;
	}

	if (m_et->cfg->sym_count == 1) {
		const_cast<p2d_emitter_cfg*>(m_et->cfg)->sym_count = 0;
	} else {
		const p2d_symbol& src = m_et->cfg->syms[--const_cast<p2d_emitter_cfg*>(m_et->cfg)->sym_count];
		p2d_symbol& dst = m_et->cfg->syms[idx];
		memcpy(&dst, &src, SIZEOF_P2D_SYMBOL);
	}
}

void ParticleSystem::DelAllSymbol()
{
	const_cast<p2d_emitter_cfg*>(m_et->cfg)->sym_count = 0;
}

const p2d_emitter_cfg* ParticleSystem::GetConfig() const
{
	return m_et->cfg;
}

}