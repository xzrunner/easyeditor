#include "ParticleSystem.h"
#include "Symbol.h"
#include "config.h"
#include "AnimRecorder.h"
#include "InvertRecord.h"
#include "ps_config.h"
#include "utility.h"

#include <particle3d.h>

namespace eparticle3d
{


ParticleSystem* ParticleSystem::PS = NULL;

ParticleSystem::ParticleSystem(unsigned int buffer, p3d_ps_config* cfg)
	: m_anim_recorder(new AnimRecorder(buffer))
	, m_inv_record(new InvertRecord)
{
	PS = this;

	m_ps = p3d_create(buffer, cfg);

	m_ps->add_func = &AddFunc;
	m_ps->remove_func = &RemoveFunc;
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
{
	PS = this;

	m_pos = ps.m_pos;
	m_ps = p3d_create(ps.GetPSCapacity(), ps.m_ps->cfg);

	m_ps->add_func = &AddFunc;
	m_ps->remove_func = &RemoveFunc;
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
	case PS_CAPACITY:
		SetPSCapacity(data.val0);
		break;
	case PS_COUNT:
		m_ps->cfg->count = data.val0;
		break;
	case PS_EMISSION_TIME:
		m_ps->cfg->emission_time = data.val0 * 0.001f; 
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
		m_ps->cfg->angular_spd = data.val0 * d2d::TRANS_DEG_TO_RAD;
		m_ps->cfg->angular_spd_var = data.val1 * d2d::TRANS_DEG_TO_RAD;
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
	case PS_CAPACITY:
		data.val0 = GetPSCapacity();
		break;
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
		data.val0 = m_ps->cfg->angular_spd * d2d::TRANS_RAD_TO_DEG;
		data.val1 = m_ps->cfg->angular_spd_var * d2d::TRANS_RAD_TO_DEG;
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
	p3d_update(m_ps, dt);
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
	m_ps->emit_counter = 0;
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
	if (loop == m_ps->cfg->loop) {
		return;
	}

	m_ps->cfg->loop = loop;

	if (m_ps->cfg->loop) {
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

void ParticleSystem::SetBounce(bool open) 
{ 
	m_ps->cfg->bounce = open; 
}

void ParticleSystem::SetOrientToMovement(bool open) 
{ 
	m_ps->cfg->orient_to_movement = open;
}

void ParticleSystem::SetRadius3D(bool is3d) 
{ 
	m_ps->cfg->is_start_radius_3d = is3d;
}

p3d_symbol* ParticleSystem::AddSymbol(d2d::ISymbol* symbol)
{
	assert(m_ps->cfg->symbol_count < MAX_COMPONENTS);

	p3d_symbol& comp = m_ps->cfg->symbols[m_ps->cfg->symbol_count++];
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
	if (idx < 0 || idx >= m_ps->cfg->symbol_count) {
		return;
	}

	if (m_ps->cfg->symbol_count == 1) {
		m_ps->cfg->symbol_count = 0;
	} else {
		const p3d_symbol& src = m_ps->cfg->symbols[--m_ps->cfg->symbol_count];
		p3d_symbol& dst = m_ps->cfg->symbols[idx];
		memcpy(&dst, &src, sizeof(p3d_symbol));
	}
}

void ParticleSystem::DelAllSymbol()
{
	m_ps->cfg->symbol_count = 0;
}

const p3d_ps_config* ParticleSystem::GetConfig() const
{
	return m_ps->cfg;
}

int ParticleSystem::GetPSCapacity() const
{
	return m_ps->end - m_ps->start;
}

void ParticleSystem::Draw(p3d_particle_system* ps, const d2d::Matrix& mt, AnimRecorder* recorder) const
{
	if (m_anim_recorder) {
		m_anim_recorder->FinishFrame();
	}

	p3d_particle* p = ps->start;
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
		_mt.translate(p->init_pos.x, p->init_pos.y);
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

void ParticleSystem::SetPSCapacity(int cap)
{	
	if (cap <= 0) {
		return;
	}

	int curr_cap = GetPSCapacity();
	if (cap == curr_cap) {
		return;
	} else {
		Reset();
	}

	if (cap < curr_cap) {
		m_ps->end = m_ps->start + cap;
	} else if (cap > curr_cap) {
		int real_cap = curr_cap;
		while (real_cap < cap) {
			real_cap *= 2;
		}
		p3d_particle_system* new_ps = p3d_create(real_cap, m_ps->cfg);
		p3d_release(m_ps);
		m_ps = new_ps;

		m_ps->add_func = &AddFunc;
		m_ps->remove_func = &RemoveFunc;
	}
}

void ParticleSystem::AddFunc(p3d_particle* p)
{
	p->init_pos.x = PS->m_pos.x;
	p->init_pos.y = PS->m_pos.y;
}

void ParticleSystem::RemoveFunc(p3d_particle* p)
{
	if (PS->m_inv_record) {
		PS->m_inv_record->AddItem(p);
	}
}

}
