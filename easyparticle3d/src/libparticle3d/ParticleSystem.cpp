#include "ParticleSystem.h"
#include "AnimRecorder.h"
#include "InvertRecord.h"
#include "ps_config.h"
#include "PSNode.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>

#include <ee/Math2D.h>
#include <ee/Symbol.h>

#include <assert.h>

namespace eparticle3d
{

ParticleSystem::ParticleSystem(p3d_emitter_cfg* cfg, bool record)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
{
	PSNode::Instance();

	if (record) {
		m_anim_recorder = new AnimRecorder(4096);
		m_inv_record = new InvertRecord;
	}

	Init(cfg);
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
{
	PSNode::Instance();

	Init(ps.m_spr->et->cfg);
}

ParticleSystem::~ParticleSystem()
{
	delete m_anim_recorder;
	delete m_inv_record;
	p3d_sprite_release(m_spr);	
}

void ParticleSystem::SetValue(int key, const ee::UICallback::Data& data)
{
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_spr->et->cfg);

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
	case PS_RADIAL_SPEED:
		cfg->radial_spd = data.val0 * 0.25f;
		cfg->radial_spd_var = data.val1 * 0.25f;
		break;
	case PS_TANGENTIAL_SPEED:
		cfg->tangential_spd = data.val0;
		cfg->tangential_spd_var = data.val1;
		break;
	case PS_ANGULAR_SPEED:
		cfg->angular_spd = data.val0 * SM_DEG_TO_RAD;
		cfg->angular_spd_var = data.val1 * SM_DEG_TO_RAD;
		break;
	case PS_DISTURBANCE_RADIUS:
		cfg->dis_region = data.val0;
		cfg->dis_region_var = data.val1;
		break;
	case PS_DISTURBANCE_SPD:
		cfg->dis_spd = data.val0;
		cfg->dis_spd_var = data.val1;
		break;
	case PS_GRAVITY:
		cfg->gravity = data.val0 * 0.3f;
		if (m_inv_record) {
			m_inv_record->SetGravity(cfg->gravity);
		}
		break;
	case PS_LINEAR_ACC:
		cfg->linear_acc = data.val0;
		cfg->linear_acc_var = data.val1;
		break;
	case PS_FADEOUT_TIME:
		cfg->fadeout_time = data.val0 * 0.001f;
		break;
	case PS_START_POS:
		cfg->start_radius = data.val0;
		cfg->start_height = data.val1;
		break;
	}
}

void ParticleSystem::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case PS_COUNT:
		data.val0 = static_cast<float>(m_spr->et->cfg->count);
		break;
	case PS_EMISSION_TIME:
		data.val0 = m_spr->et->cfg->emission_time * 1000;
		break;
	case PS_LIFE_TIME:
		data.val0 = m_spr->et->cfg->life * 1000;
		data.val1 = m_spr->et->cfg->life_var * 1000;
		break;
	case PS_RADIAL_SPEED:
		data.val0 = m_spr->et->cfg->radial_spd * 4;
		data.val1 = m_spr->et->cfg->radial_spd_var * 4;
		break;
	case PS_TANGENTIAL_SPEED:
		data.val0 = m_spr->et->cfg->tangential_spd;
		data.val1 = m_spr->et->cfg->tangential_spd_var;
		break;
	case PS_ANGULAR_SPEED:
		data.val0 = m_spr->et->cfg->angular_spd * SM_RAD_TO_DEG;
		data.val1 = m_spr->et->cfg->angular_spd_var * SM_RAD_TO_DEG;
		break;
	case PS_DISTURBANCE_RADIUS:
		data.val0 = m_spr->et->cfg->dis_region;
		data.val1 = m_spr->et->cfg->dis_region_var;
		break;
	case PS_DISTURBANCE_SPD:
		data.val0 = m_spr->et->cfg->dis_spd;
		data.val1 = m_spr->et->cfg->dis_spd_var;
		break;
	case PS_GRAVITY:
		data.val0 = m_spr->et->cfg->gravity / 0.3f;
		break;
	case PS_LINEAR_ACC:
		data.val0 = m_spr->et->cfg->linear_acc;
		data.val1 = m_spr->et->cfg->linear_acc_var;
		break;
	case PS_FADEOUT_TIME:
		data.val0 = m_spr->et->cfg->fadeout_time * 1000;
		break;
	case PS_START_POS:
		data.val0 = m_spr->et->cfg->start_radius;
		data.val1 = m_spr->et->cfg->start_height;
		break;
	}
}

void ParticleSystem::Draw(const sm::mat4& mt, AnimRecorder* recorder) const
{
//  // todo record
// 	if (m_anim_recorder) {
// 		m_anim_recorder->FinishFrame();
// 	}

	m_rp.mat = const_cast<sm::mat4&>(mt);
	p3d_emitter_draw(m_spr->et, &m_rp);
}

bool ParticleSystem::Update(const sm::mat4& mat)
{
	float time = PSNode::Instance()->GetTime();
	assert(m_spr->et->time <= time);
	if (m_spr->et->time == time) {
		return false;
	}

	float mt[6];
	mt[0] = mat.x[0];
	mt[1] = mat.x[1];
	mt[2] = mat.x[4];
	mt[3] = mat.x[5];
	mt[4] = mat.x[12];
	mt[5] = mat.x[13];	

	float dt = time - m_spr->et->time;
	p3d_emitter_update(m_spr->et, dt, mt);
	m_spr->et->time = time;

	return true;
}

void ParticleSystem::SetStaticMode(bool is_static)
{
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_spr->et->cfg);
	cfg->static_mode = is_static;
}

void ParticleSystem::SetDirection(float x, float y, float z)
{
	sm::vec3 start(0, 0, 1), end(x, y, z);
	end.Normalize();
	SetDirection(sm::Quaternion::CreateFromVectors(start, end));
}

void ParticleSystem::SetDirection(const sm::Quaternion& dir)
{
	// todo
//	direction = dir.ToMatrix();
}

void ParticleSystem::Start()
{
	p3d_emitter_start(m_spr->et);
}

void ParticleSystem::Stop()
{
	p3d_emitter_stop(m_spr->et);
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
	p3d_emitter_pause(m_spr->et);
}

void ParticleSystem::SetLoop(bool loop)
{
	if (loop == m_spr->et->loop) {
		return;
	}

	m_spr->et->loop = loop;

	Start();
// 	if (m_spr->et->loop) {
// 		Start();
// 	} else {
// 		Pause();
// 	}
}

void ParticleSystem::SetLocalModeDraw(bool local)
{
	m_spr->local_mode_draw = local;
}

bool ParticleSystem::IsLocalModeDraw() const
{
	return m_spr->local_mode_draw;
}

void ParticleSystem::Clear()
{
	p3d_emitter_clear(m_spr->et);
	if (m_anim_recorder) {
		m_anim_recorder->Clear();
	}
	if (m_inv_record) {
		m_inv_record->Clear();
	}
}

bool ParticleSystem::IsEmpty() const
{
	return !m_spr->et->head;
}

void ParticleSystem::ReloadTexture() const
{
	for (int i = 0; i < m_spr->et->cfg->symbol_count; ++i) {
		ee::Symbol* symbol = static_cast<ee::Symbol*>(m_spr->et->cfg->symbols[i].ud);
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
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_spr->et->cfg);
	cfg->hori = (min + max) * 0.5f * SM_DEG_TO_RAD;
	cfg->hori_var = (max - min) * 0.5f * SM_DEG_TO_RAD;
}

void ParticleSystem::SetVert(int min, int max) 
{ 
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_spr->et->cfg);
	cfg->vert = (min + max) * 0.5f * SM_DEG_TO_RAD;
	cfg->vert_var = (max - min) * 0.5f * SM_DEG_TO_RAD;
}

void ParticleSystem::SetGround(int ground)
{
	const_cast<p3d_emitter_cfg*>(m_spr->et->cfg)->ground = ground;
}

void ParticleSystem::SetOrientToMovement(bool open) 
{ 
	const_cast<p3d_emitter_cfg*>(m_spr->et->cfg)->orient_to_movement = open;
}

p3d_symbol* ParticleSystem::AddSymbol(ee::Symbol* symbol)
{
	assert(m_spr->et->cfg->symbol_count < MAX_COMPONENTS);

	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_spr->et->cfg);
	p3d_symbol& comp = m_spr->et->cfg->symbols[cfg->symbol_count++];
	memset(&comp, 0, SIZEOF_P3D_SYMBOL);

	comp.scale_start = comp.scale_end = 1;

	comp.mul_col_begin.r = comp.mul_col_begin.g = comp.mul_col_begin.b = comp.mul_col_begin.a = 255;
	comp.mul_col_end.r = comp.mul_col_end.g = comp.mul_col_end.b = comp.mul_col_end.a = 255;
	comp.add_col_begin.r = comp.add_col_begin.g = comp.add_col_begin.b = comp.add_col_begin.a = 0;
	comp.add_col_end.r = comp.add_col_end.g = comp.add_col_end.b = comp.add_col_end.a = 0;

	comp.ud = symbol;

	return &comp;
}

void ParticleSystem::DelSymbol(int idx)
{
	if (idx < 0 || idx >= m_spr->et->cfg->symbol_count) {
		return;
	}

	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_spr->et->cfg);
	if (cfg->symbol_count == 1) {
		cfg->symbol_count = 0;
	} else {
		for (int i = idx; i < cfg->symbol_count - 1; ++i) {
			const p3d_symbol* src = &cfg->symbols[i+1];
			p3d_symbol* dst = &cfg->symbols[i];
			memcpy(dst, src, sizeof(p3d_symbol));
		}
		--cfg->symbol_count;
	}
}

void ParticleSystem::DelAllSymbol()
{
	const_cast<p3d_emitter_cfg*>(m_spr->et->cfg)->symbol_count = 0;
}

p3d_symbol* ParticleSystem::GetSymbol(int idx)
{
	if (idx < 0 || idx >= m_spr->et->cfg->symbol_count) {
		return NULL;
	} else {
		return &m_spr->et->cfg->symbols[idx];
	}
}

const p3d_emitter_cfg* ParticleSystem::GetConfig() const
{
	return m_spr->et->cfg;
}

const p3d_emitter* ParticleSystem::GetEmitter() const 
{ 
	return m_spr->et; 
}

void ParticleSystem::Init(const p3d_emitter_cfg* cfg)
{
	m_spr = p3d_sprite_create();
	assert(m_spr);
	m_spr->et = p3d_emitter_create(cfg);
	m_spr->et->ud = this;
	m_spr->local_mode_draw = true;
	memset(m_spr->mat, 0, sizeof(m_spr->mat));
	m_spr->ptr_self = &m_spr;

	m_rp.p3d = m_spr;
}

}
