#include "ParticleSystem.h"
#include "AnimRecorder.h"
#include "InvertRecord.h"
#include "ps_config.h"
#include "PS.h"

#include <ps_3d.h>

namespace eparticle3d
{

ParticleSystem::ParticleSystem(p3d_emitter_cfg* cfg, bool record)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
{
	PS::Instance();

	if (record) {
		m_anim_recorder = new AnimRecorder(4096);
		m_inv_record = new InvertRecord;
	}

	m_et = p3d_emitter_create(cfg);
	m_et->ud = this;
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
{
	PS::Instance();

	m_et = p3d_emitter_create(ps.m_et->cfg);
	m_et->ud = this;
}

ParticleSystem::~ParticleSystem()
{
	delete m_anim_recorder;
	delete m_inv_record;
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
	case PS_SPEED:
		m_et->cfg->spd = data.val0 * 0.25f;
		m_et->cfg->spd_var = data.val1 * 0.25f;
		break;
	case PS_ANGULAR_SPEED:
		m_et->cfg->angular_spd = data.val0 * d2d::TRANS_DEG_TO_RAD;
		m_et->cfg->angular_spd_var = data.val1 * d2d::TRANS_DEG_TO_RAD;
		break;
	case PS_DISTURBANCE_RADIUS:
		m_et->cfg->dis_region = data.val0;
		m_et->cfg->dis_region_var = data.val1;
		break;
	case PS_DISTURBANCE_SPD:
		m_et->cfg->dis_spd = data.val0;
		m_et->cfg->dis_spd_var = data.val1;
		break;
	case PS_GRAVITY:
		m_et->cfg->gravity = data.val0 * 0.3f;
		if (m_inv_record) {
			m_inv_record->SetGravity(m_et->cfg->gravity);
		}
		break;
	case PS_LINEAR_ACC:
		m_et->cfg->linear_acc = data.val0;
		m_et->cfg->linear_acc_var = data.val1;
		break;
	case PS_FADEOUT_TIME:
		m_et->cfg->fadeout_time = data.val0 * 0.001f;
		break;
	case PS_START_POS:
		m_et->cfg->start_radius = data.val0;
		m_et->cfg->start_height = data.val1;
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
	case PS_SPEED:
		data.val0 = m_et->cfg->spd * 4;
		data.val1 = m_et->cfg->spd_var * 4;
		break;
	case PS_ANGULAR_SPEED:
		data.val0 = m_et->cfg->angular_spd * d2d::TRANS_RAD_TO_DEG;
		data.val1 = m_et->cfg->angular_spd_var * d2d::TRANS_RAD_TO_DEG;
		break;
	case PS_DISTURBANCE_RADIUS:
		data.val0 = m_et->cfg->dis_region;
		data.val1 = m_et->cfg->dis_region_var;
		break;
	case PS_DISTURBANCE_SPD:
		data.val0 = m_et->cfg->dis_spd;
		data.val1 = m_et->cfg->dis_spd_var;
		break;
	case PS_GRAVITY:
		data.val0 = m_et->cfg->gravity / 0.3f;
		break;
	case PS_LINEAR_ACC:
		data.val0 = m_et->cfg->linear_acc;
		data.val1 = m_et->cfg->linear_acc_var;
		break;
	case PS_FADEOUT_TIME:
		data.val0 = m_et->cfg->fadeout_time * 1000;
		break;
	case PS_START_POS:
		data.val0 = m_et->cfg->start_radius;
		data.val1 = m_et->cfg->start_height;
		break;
	}
}

void ParticleSystem::Draw(const d2d::Matrix& mt, AnimRecorder* recorder) const
{
//  // todo record
// 	if (m_anim_recorder) {
// 		m_anim_recorder->FinishFrame();
// 	}

	if (m_et->local_mode_draw) {
		p3d_emitter_draw(m_et, &mt);
	} else {
		p3d_emitter_draw(m_et, NULL);
	}
}

bool ParticleSystem::Update(const d2d::Matrix& mat)
{
	float time = PS::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	const float* src = mat.getElements();
	float mt[6];
	mt[0] = src[0];
	mt[1] = src[1];
	mt[2] = src[4];
	mt[3] = src[5];
	mt[4] = src[12];
	mt[5] = src[13];	

	float dt = time - m_et->time;
	p3d_emitter_update(m_et, dt, mt);
	m_et->time = time;

	return true;
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
	m_et->active = true;
	m_et->particle_count = 0;
}

void ParticleSystem::Stop()
{
	m_et->active = false;
	m_et->emit_counter = 0;
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

void ParticleSystem::SetLocalModeDraw(bool local)
{
	m_et->local_mode_draw = local;
}

void ParticleSystem::Clear()
{
	p3d_emitter_clear(m_et);
	if (m_anim_recorder) {
		m_anim_recorder->Clear();
	}
	if (m_inv_record) {
		m_inv_record->Clear();
	}
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

void ParticleSystem::RemoveFromInvertRecord(p3d_particle* p) 
{
	if (m_inv_record) {
		m_inv_record->AddItem(p);
	}	
}

void ParticleSystem::SetHori(int min, int max) 
{
	m_et->cfg->hori = (min + max) * 0.5f * d2d::TRANS_DEG_TO_RAD;
	m_et->cfg->hori_var = (max - min) * 0.5f * d2d::TRANS_DEG_TO_RAD;
}

void ParticleSystem::SetVert(int min, int max) 
{ 
	m_et->cfg->vert = (min + max) * 0.5f * d2d::TRANS_DEG_TO_RAD;
	m_et->cfg->vert_var = (max - min) * 0.5f * d2d::TRANS_DEG_TO_RAD;
}

void ParticleSystem::SetGround(int ground)
{
	m_et->cfg->ground = ground;
}

void ParticleSystem::SetOrientToMovement(bool open) 
{ 
	m_et->cfg->orient_to_movement = open;
}

p3d_symbol* ParticleSystem::AddSymbol(d2d::ISymbol* symbol)
{
	assert(m_et->cfg->symbol_count < MAX_COMPONENTS);

	p3d_symbol& comp = m_et->cfg->symbols[m_et->cfg->symbol_count++];
	memset(&comp, 0, SIZEOF_P3D_SYMBOL);

	comp.scale_start = comp.scale_end = 1;

	comp.col_mul.r = comp.col_mul.g = comp.col_mul.b = comp.col_mul.a = 1;
	comp.col_add.r = comp.col_add.g = comp.col_add.b = 0;

	comp.alpha_start = comp.alpha_end = 1;

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
		for (int i = idx; i < m_et->cfg->symbol_count - 1; ++i) {
			const p3d_symbol* src = &m_et->cfg->symbols[i+1];
			p3d_symbol* dst = &m_et->cfg->symbols[i];
			memcpy(dst, src, sizeof(p3d_symbol));
		}
		--m_et->cfg->symbol_count;
	}
}

void ParticleSystem::DelAllSymbol()
{
	m_et->cfg->symbol_count = 0;
}

p3d_symbol* ParticleSystem::GetSymbol(int idx)
{
	if (idx < 0 || idx >= m_et->cfg->symbol_count) {
		return NULL;
	} else {
		return &m_et->cfg->symbols[idx];
	}
}

const p3d_emitter_cfg* ParticleSystem::GetConfig() const
{
	return m_et->cfg;
}

}
