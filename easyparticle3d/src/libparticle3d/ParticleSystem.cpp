#include "ParticleSystem.h"
#include "AnimRecorder.h"
#include "InvertRecord.h"
#include "ps_config.h"

#include <ee/Math2D.h>
#include <ee/Symbol.h>

#include <ps_3d.h>
#include <sprite2/Particle3dEmitter.h>
#include <sprite2/P3dEmitterCfg.h>

#include <assert.h>

namespace eparticle3d
{

ParticleSystem::ParticleSystem(const s2::P3dEmitterCfg* cfg, bool record)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
	, m_et(NULL)
{
	s2::Particle3d::Instance();

	if (record) {
		m_anim_recorder = new AnimRecorder(4096);
		m_inv_record = new InvertRecord;
	}

	InitEmitter(cfg);
}

ParticleSystem::ParticleSystem(const ParticleSystem& ps)
	: m_anim_recorder(NULL)
	, m_inv_record(NULL)
	, m_et(NULL)
{
	s2::Particle3d::Instance();

	InitEmitter(ps.m_et->GetEmitterCfg());
}

ParticleSystem::~ParticleSystem()
{
	delete m_anim_recorder;
	delete m_inv_record;

	if (m_et) {
		m_et->RemoveReference();
	}
}

void ParticleSystem::SetValue(int key, const ee::UICallback::Data& data)
{
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
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
	if (!m_et) {
		return;
	}
	const p3d_emitter_cfg* cfg = m_et->GetEmitterCfg()->GetImpl();
	switch (key)
	{
	case PS_COUNT:
		data.val0 = static_cast<float>(cfg->count);
		break;
	case PS_EMISSION_TIME:
		data.val0 = cfg->emission_time * 1000;
		break;
	case PS_LIFE_TIME:
		data.val0 = cfg->life * 1000;
		data.val1 = cfg->life_var * 1000;
		break;
	case PS_RADIAL_SPEED:
		data.val0 = cfg->radial_spd * 4;
		data.val1 = cfg->radial_spd_var * 4;
		break;
	case PS_TANGENTIAL_SPEED:
		data.val0 = cfg->tangential_spd;
		data.val1 = cfg->tangential_spd_var;
		break;
	case PS_ANGULAR_SPEED:
		data.val0 = cfg->angular_spd * SM_RAD_TO_DEG;
		data.val1 = cfg->angular_spd_var * SM_RAD_TO_DEG;
		break;
	case PS_DISTURBANCE_RADIUS:
		data.val0 = cfg->dis_region;
		data.val1 = cfg->dis_region_var;
		break;
	case PS_DISTURBANCE_SPD:
		data.val0 = cfg->dis_spd;
		data.val1 = cfg->dis_spd_var;
		break;
	case PS_GRAVITY:
		data.val0 = cfg->gravity / 0.3f;
		break;
	case PS_LINEAR_ACC:
		data.val0 = cfg->linear_acc;
		data.val1 = cfg->linear_acc_var;
		break;
	case PS_FADEOUT_TIME:
		data.val0 = cfg->fadeout_time * 1000;
		break;
	case PS_START_POS:
		data.val0 = cfg->start_radius;
		data.val1 = cfg->start_height;
		break;
	}
}

void ParticleSystem::Draw(AnimRecorder* recorder) const
{
//  // todo record
// 	if (m_anim_recorder) {
// 		m_anim_recorder->FinishFrame();
// 	}

	if (m_et) {
		m_et->Draw(s2::P3dRenderParams(), true);
	}

//	sl::ShaderMgr::Instance()->GetContext()->SetDefaultBlend();
}

bool ParticleSystem::Update(const S2_MAT& mat)
{
	if (!m_et) {
		return false;
	}

	float mt[6];
	mt[0] = mat.x[0];
	mt[1] = mat.x[1];
	mt[2] = mat.x[4];
	mt[3] = mat.x[5];
	mt[4] = mat.x[12];
	mt[5] = mat.x[13];	
	m_et->SetMat(mt);

	float time = s2::Particle3d::Instance()->GetTime();
	return m_et->Update(time);
}

void ParticleSystem::SetStaticMode(bool is_static)
{
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
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
	if (m_et) {
		m_et->Start();
	}
}

void ParticleSystem::Stop()
{
	if (m_et) {
		m_et->Stop();
	}
}

void ParticleSystem::Reset()
{
	Clear();
}

void ParticleSystem::Pause()
{
	if (m_et) {
		m_et->Pause();
	}
}

void ParticleSystem::SetLoop(bool loop)
{
	if (!m_et) {
		return;
	}

	if (m_et->IsLoop() == loop) {
		return;
	}

	m_et->SetLoop(loop);
	m_et->Start();

// 	if (m_spr->et->loop) {
// 		Start();
// 	} else {
// 		Pause();
// 	}
}

bool ParticleSystem::IsLoop() const
{
	if (m_et) {
		return m_et->IsLoop();
	} else {
		return true;
	}
}

void ParticleSystem::SetLocal(bool local)
{
	if (m_et) {
		m_et->SetLocal(local);
	}
}

bool ParticleSystem::IsLocal() const
{
	if (m_et) {
		return m_et->IsLocal();
	} else {
		return true;
	}
}

void ParticleSystem::Clear()
{
	if (m_et) {
		m_et->Clear();
	}
	if (m_anim_recorder) {
		m_anim_recorder->Clear();
	}
	if (m_inv_record) {
		m_inv_record->Clear();
	}
}

bool ParticleSystem::IsEmpty() const
{
	if (m_et) {
		return m_et->IsFinished();
	} else {
		return true;
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
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	cfg->hori = (min + max) * 0.5f * SM_DEG_TO_RAD;
	cfg->hori_var = (max - min) * 0.5f * SM_DEG_TO_RAD;
}

void ParticleSystem::SetVert(int min, int max) 
{ 
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	cfg->vert = (min + max) * 0.5f * SM_DEG_TO_RAD;
	cfg->vert_var = (max - min) * 0.5f * SM_DEG_TO_RAD;
}

void ParticleSystem::SetGround(int ground)
{
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	cfg->ground = ground;
}

void ParticleSystem::SetOrientToMovement(bool open) 
{
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	cfg->orient_to_movement = open;
}

void ParticleSystem::SetBlend(int blend)
{
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	cfg->blend = blend;
}

p3d_symbol* ParticleSystem::AddSymbol(s2::Symbol* sym)
{
	if (!m_et) {
		return NULL;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	assert(cfg->sym_count < MAX_COMPONENTS);

	p3d_symbol& comp = cfg->syms[cfg->sym_count++];
	memset(&comp, 0, SIZEOF_P3D_SYMBOL);

	comp.scale_start = comp.scale_end = 1;

	comp.mul_col_begin.r = comp.mul_col_begin.g = comp.mul_col_begin.b = comp.mul_col_begin.a = 255;
	comp.mul_col_end.r = comp.mul_col_end.g = comp.mul_col_end.b = comp.mul_col_end.a = 255;
	comp.add_col_begin.r = comp.add_col_begin.g = comp.add_col_begin.b = comp.add_col_begin.a = 0;
	comp.add_col_end.r = comp.add_col_end.g = comp.add_col_end.b = comp.add_col_end.a = 0;

	sym->AddReference();
	comp.ud = sym;

	return &comp;
}

void ParticleSystem::DelSymbol(int idx)
{
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	if (!cfg || idx < 0 || idx >= cfg->sym_count) {
		return;
	}

	p3d_symbol& comp = cfg->syms[idx];
	static_cast<s2::Symbol*>(comp.ud)->RemoveReference();
	if (cfg->sym_count == 1) {
		cfg->sym_count = 0;
	} else {
		for (int i = idx; i < cfg->sym_count - 1; ++i) {
			const p3d_symbol* src = &cfg->syms[i+1];
			p3d_symbol* dst = &cfg->syms[i];
			memcpy(dst, src, sizeof(p3d_symbol));
		}
		--cfg->sym_count;
	}
}

void ParticleSystem::DelAllSymbol()
{
	if (!m_et) {
		return;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	for (int i = 0; i < cfg->sym_count; ++i) {
		s2::Symbol* sym = (s2::Symbol*)(cfg->syms[i].ud);
		sym->RemoveReference();
	}
	cfg->sym_count = 0;
}

p3d_symbol* ParticleSystem::GetSymbol(int idx)
{
	if (!m_et) {
		return NULL;
	}
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	if (idx < 0 || idx >= cfg->sym_count) {
		return NULL;
	} else {
		return &cfg->syms[idx];
	}
}

const s2::P3dEmitterCfg* ParticleSystem::GetConfig() const
{
	if (m_et) {
		return m_et->GetEmitterCfg();
	} else {
		return NULL;
	}
}

//
//const p3d_emitter* ParticleSystem::GetEmitter() const 
//{ 
//}

void ParticleSystem::InitEmitter(const s2::P3dEmitterCfg* cfg)
{
	if (!m_et) {
		m_et = s2::P3dEmitterPool::Instance()->Pop();
	}
	m_et->CreateEmitter(cfg);
	m_et->Start();
}

}
