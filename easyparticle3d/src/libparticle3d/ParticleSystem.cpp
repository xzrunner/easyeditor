#include "ParticleSystem.h"
#include "Symbol.h"
#include "config.h"
#include "AnimRecorder.h"
#include "InvertRecord.h"
#include "ps_config.h"
#include "utility.h"

#include <ps/particle3d.h>

namespace eparticle3d
{

ParticleSystem::ParticleSystem(unsigned int buffer, ps_cfg_3d* cfg)
	: m_anim_recorder(new AnimRecorder(buffer))
	, m_inv_record(new InvertRecord)
{
	m_ps = ps_create(buffer, cfg);
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
{
	m_origin = ps.m_origin;
	m_ps = ps_create(PARTICLE_CAP, ps.m_ps->cfg);
}

ParticleSystem::~ParticleSystem()
{
	delete m_anim_recorder;
	delete m_inv_record;

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
		m_ps->cfg->lifetime = m_ps->cfg->emission_time = data.val0 * 0.001f; 
		break;
	case PS_LIFE_TIME:
		m_ps->cfg->life = data.val0 * 0.001f;
		m_ps->cfg->life_var = data.val1 * 0.001f;
		break;
	case PS_SPEED:
		m_ps->cfg->spd = data.val0 * 0.25f;
		m_ps->cfg->spd_var = data.val1 * 0.25f;
		break;
	case PS_ANGULAR_SPEED:
		m_ps->cfg->angular_spd = data.val0;
		m_ps->cfg->angular_spd_var = data.val1;
		break;
	case PS_DISTURBANCE_RADIUS:
		m_ps->cfg->dis_region = data.val0;
		m_ps->cfg->dis_region_var = data.val1;
		break;
	case PS_DISTURBANCE_SPD:
		m_ps->cfg->dis_spd = data.val0;
		m_ps->cfg->dis_spd_var = data.val1;
		break;
	case PS_GRAVITY:
		m_ps->cfg->gravity = data.val0 * 0.3f;
		m_inv_record->SetGravity(m_ps->cfg->gravity);
		break;
	case PS_LINEAR_ACC:
		m_ps->cfg->linear_acc = data.val0;
		m_ps->cfg->linear_acc_var = data.val1;
		break;
	case PS_FADEOUT_TIME:
		m_ps->cfg->fadeout_time = data.val0 * 0.001f;
		break;
	case PS_START_RADIUS:
		m_ps->cfg->start_radius = data.val0;
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
		data.val0 = m_ps->cfg->emission_time * 1000;
		break;
	case PS_LIFE_TIME:
		data.val0 = m_ps->cfg->life * 1000;
		data.val1 = m_ps->cfg->life_var * 1000;
		break;
	case PS_SPEED:
		data.val0 = m_ps->cfg->spd * 4;
		data.val1 = m_ps->cfg->spd_var * 4;
		break;
	case PS_ANGULAR_SPEED:
		data.val0 = m_ps->cfg->angular_spd;
		data.val1 = m_ps->cfg->angular_spd_var;
		break;
	case PS_DISTURBANCE_RADIUS:
		data.val0 = m_ps->cfg->dis_region;
		data.val1 = m_ps->cfg->dis_region_var;
		break;
	case PS_DISTURBANCE_SPD:
		data.val0 = m_ps->cfg->dis_spd;
		data.val1 = m_ps->cfg->dis_spd_var;
		break;
	case PS_GRAVITY:
		data.val0 = m_ps->cfg->gravity / 0.3f;
		break;
	case PS_LINEAR_ACC:
		data.val0 = m_ps->cfg->linear_acc;
		data.val1 = m_ps->cfg->linear_acc_var;
		break;
	case PS_FADEOUT_TIME:
		data.val0 = m_ps->cfg->fadeout_time * 1000;
		break;
	case PS_START_RADIUS:
		data.val0 = m_ps->cfg->start_radius;
		break;
	}
}

void ParticleSystem::Draw(const d2d::Matrix& mt, AnimRecorder* recorder) const
{
	Draw(m_ps, mt, recorder);
}

void ParticleSystem::Update(float dt)
{
	ps_update(m_ps, dt);
}

void ParticleSystem::SetDirection(float x, float y, float z)
{
	vec3 start(0, 0, 1), end(x, y, z);
	end.Normalize();
	SetDirection(Quaternion::CreateFromVectors(start, end));
}

void ParticleSystem::SetDirection(const Quaternion& dir)
{
	// todo
//	direction = dir.ToMatrix();
}

void ParticleSystem::Start()
{
	m_ps->active = true;
}

void ParticleSystem::Stop()
{
	m_ps->active = false;
	m_ps->life = m_ps->cfg->lifetime;
	m_ps->emit_counter = 0;
}

void ParticleSystem::Clear()
{
	m_ps->last = m_ps->start;
}

void ParticleSystem::Reset()
{
//  	// todo clear child's ps
//  	Particle* p = pStart;
//  	while (p != pLast)
//  	{
//  		if (p->pc->m_bind_ps) {
//  			p->pc->m_bind_ps->reset();
// 			p->pc->m_bind_ps->stop();
//  		}
//  		++p;
//  	}

	m_ps->last = m_ps->start;
	m_ps->life = m_ps->cfg->lifetime = m_ps->cfg->emission_time; // m_ps->cfg->lifetime = m_ps->cfg->emission_time?
	m_ps->emit_counter = 0;

	if (m_anim_recorder) {
		m_anim_recorder->Clear();
	}
	if (m_inv_record) {
		m_inv_record->Clear();
	}
}

void ParticleSystem::Pause()
{
	m_ps->active = false;
}

void ParticleSystem::SetLoop(bool loop)
{
	if (loop == m_ps->loop) {
		return;
	}

	m_ps->life = m_ps->cfg->lifetime = m_ps->cfg->emission_time;
	m_ps->loop = loop;

	if (m_ps->loop) {
		Start();
	}
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

void ParticleSystem::StoreAnimRecord(const std::string& filepath) const
{
	if (m_anim_recorder) {
		m_anim_recorder->StoreToFile(filepath);
	}
}

void ParticleSystem::StoreInvertRecord(const std::string& filepath) const
{
	if (m_inv_record) {
		m_inv_record->StoreToFile(filepath);
	}
}

void ParticleSystem::SetHori(int min, int max) 
{
	m_ps->cfg->hori = (min + max) * 0.5f * d2d::TRANS_DEG_TO_RAD;
	m_ps->cfg->hori_var = (max - min) * 0.5f * d2d::TRANS_DEG_TO_RAD;
}

void ParticleSystem::SetVert(int min, int max) 
{ 
	m_ps->cfg->vert = (min + max) * 0.5f * d2d::TRANS_DEG_TO_RAD;
	m_ps->cfg->vert_var = (max - min) * 0.5f * d2d::TRANS_DEG_TO_RAD;
}

void ParticleSystem::SetInertia(int val) 
{ 
//	inertia = val * d2d::TRANS_DEG_TO_RAD; 
}

void ParticleSystem::SetBounce(bool open) 
{ 
	m_ps->cfg->bounce = open; 
}

void ParticleSystem::SetAdditiveBlend(bool open) 
{ 
//	additive_blend = open; 
}

void ParticleSystem::SetOrientToMovement(bool open) 
{ 
	m_ps->cfg->orient_to_movement = open;
}

void ParticleSystem::SetRadius3D(bool is3d) 
{ 
	m_ps->cfg->is_start_radius_3d = is3d;
}

particle_symbol* ParticleSystem::AddSymbol(d2d::ISymbol* symbol)
{
	assert(m_ps->cfg->count < MAX_COMPONENTS);

	particle_symbol& comp = m_ps->cfg->symbols[m_ps->cfg->symbol_count++];
	memset(&comp, 0, sizeof(particle_symbol));

	comp.scale_start = comp.scale_end = 1;

	comp.col_mul.r = comp.col_mul.g = comp.col_mul.b = comp.col_mul.a = 1;
	comp.col_add.r = comp.col_add.g = comp.col_add.b = 0;

	comp.alpha_start = comp.alpha_end = 1;

	comp.ud = symbol;

	return &comp;
}

void ParticleSystem::DelSymbol(int idx)
{
	if (idx < 0 || idx >= m_ps->cfg->count) {
		return;
	}

	if (m_ps->cfg->count == 1) {
		m_ps->cfg->count = 0;
	} else {
		m_ps->cfg->symbols[idx] = m_ps->cfg->symbols[--m_ps->cfg->count];
	}
}

void ParticleSystem::DelAllSymbol()
{
	m_ps->cfg->symbol_count = 0;
}

void ParticleSystem::Draw(particle_system_3d* ps, const d2d::Matrix& mt, AnimRecorder* recorder) const
{
	if (m_anim_recorder) {
		m_anim_recorder->FinishFrame();
	}

	particle_3d* p = ps->start;
	while (p != ps->last)
	{
		//glPushAttrib(GL_CURRENT_BIT);

		d2d::Colorf mul_col, add_col;
		memcpy(&mul_col.r, &p->cfg.symbol->col_mul.r, sizeof(float) * 4);
		memcpy(&add_col.r, &p->cfg.symbol->col_add.r, sizeof(float) * 4);
		if (p->life < ps->cfg->fadeout_time) {
			mul_col.a = p->life / ps->cfg->fadeout_time;
		}

		//		d2d::Vector pos = TransCoords3To2(p->position, direction);
		d2d::Vector pos = TransCoords3To2(p->pos.xyz);

		float proc = (p->cfg.lifetime - p->life) / p->cfg.lifetime;
		float s = proc * (p->cfg.symbol->scale_end - p->cfg.symbol->scale_start) + p->cfg.symbol->scale_start;
		float alpha = proc * (p->cfg.symbol->alpha_end - p->cfg.symbol->alpha_start) + p->cfg.symbol->alpha_start;
		mul_col.a *= alpha;

		d2d::Matrix _mt(mt);
		_mt.translate(p->pos.x, p->pos.y);
		d2d::ISymbol* symbol = static_cast<d2d::ISymbol*>(p->cfg.symbol->ud);
		d2d::SpriteRenderer::Instance()->Draw(symbol, _mt, pos, p->angle, s, s, 0, 0, mul_col, add_col);

		if (p->bind_ps) {
			d2d::Matrix _mt;
			_mt.translate(p->pos.x, p->pos.y);
			Draw(p->bind_ps, _mt, recorder);
		}

		AnimRecorder* curr_record = m_anim_recorder ? m_anim_recorder : recorder;
		if (curr_record) {
			d2d::Vector fixed = d2d::Math::transVector(pos, _mt);
			curr_record->AddItem(symbol->GetFilepath(), fixed.x, fixed.y, p->angle, s, mul_col, add_col);
		}

		//glPopAttrib();

		++p;
	}
}

}
