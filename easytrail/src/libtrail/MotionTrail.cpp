#include "MotionTrail.h"
#include "mt_config.h"

#include <sprite2/TrailEmitter.h>
#include <sprite2/TrailEmitterCfg.h>
#include <sprite2/S2_Symbol.h>

#include <mt_2d.h>

#include <assert.h>

namespace etrail
{

MotionTrail::MotionTrail(s2::TrailEmitterCfg* cfg)
	: m_et(NULL)
{
	s2::Trail::Instance();

	Init(cfg);
}

MotionTrail::~MotionTrail()
{
	if (m_et) {
		m_et->RemoveReference();
	}
}

void MotionTrail::SetValue(int key, const ee::UICallback::Data& data)
{
	if (!m_et) {
		return;
	}
	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	switch (key)
	{
	case MT_COUNT:
		cfg->count = static_cast<int>(data.val0);
		break;
	case MT_LIFETIME_BEGIN:
		if (data.val0 == -1) {
			cfg->life_begin = FLT_MAX;
		} else {
			cfg->life_begin = data.val0 * 0.001f;
		}
		break;
	case MT_LIFETIME_OFFSET:
		cfg->life_offset = data.val0 * 0.001f;
		break;
	case MT_FADEOUT_TIME:
		cfg->fadeout_time = data.val0 * 0.001f;
		break;
	}
}

void MotionTrail::GetValue(int key, ee::UICallback::Data& data)
{
	if (!m_et) {
		return;
	}
	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	switch (key)
	{
	case MT_COUNT:
		data.val0 = static_cast<float>(cfg->count);
		break;
	case MT_LIFETIME_BEGIN:
		if (cfg->life_begin == FLT_MAX) {
			data.val0 = -1;
		} else {
			data.val0 = cfg->life_begin * 1000;
		}
		break;
	case MT_LIFETIME_OFFSET:
		data.val0 = cfg->life_offset * 1000;
		break;
	case MT_FADEOUT_TIME:
		data.val0 = cfg->fadeout_time * 1000;
		break;
	}
}

void MotionTrail::Draw(const S2_MAT& mt) const
{
	m_rp.mat = const_cast<S2_MAT&>(mt);
	if (m_et) {
		m_et->Draw(m_rp);
	}
}

bool MotionTrail::Update(const sm::vec2& pos)
{
	if (!m_et) {
		return false;
	}

	float time = s2::Trail::Instance()->GetTime();
	return m_et->Update(time, pos);
}

void MotionTrail::Start()
{
	if (m_et) {
		m_et->Start();
	}
}

void MotionTrail::Stop()
{
	if (m_et) {
		m_et->Stop();
	}
}

void MotionTrail::Clear()
{
	if (m_et) {
		m_et->Clear();
	}
}

t2d_symbol* MotionTrail::AddSymbol(s2::Symbol* sym)
{
	if (!m_et) {
		return NULL;
	}
	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	assert(cfg->sym_count < MAX_COMPONENTS);

	t2d_symbol& comp = cfg->syms[cfg->sym_count++];
	memset(&comp, 0, SIZEOF_T2D_SYMBOL);

	comp.col_begin.r = comp.col_begin.g = comp.col_begin.b = comp.col_begin.a = 255;
	comp.col_end.r = comp.col_end.g = comp.col_end.b = comp.col_end.a = 255;
	if (cfg->mode_type == T2D_MODE_IMAGE) {
		memset(&comp.mode.A.add_col_begin, 0, sizeof(comp.mode.A.add_col_begin));
		memset(&comp.mode.A.add_col_end, 0, sizeof(comp.mode.A.add_col_end));
		comp.mode.A.scale_begin = comp.mode.A.scale_end = 1;
		comp.mode.A.ud = sym;
	} else {
		comp.mode.B.size = 1;
		comp.mode.B.acuity = 0;
	}

	return &comp;
}

void MotionTrail::DelSymbol(int idx)
{
	if (!m_et) {
		return;
	}
	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	if (idx < 0 || idx >= cfg->sym_count) {
		return;
	}

	if (cfg->sym_count == 1) {
		cfg->sym_count = 0;
	} else {
		for (int i = idx; i < cfg->sym_count - 1; ++i) {
			const t2d_symbol* src = &cfg->syms[i+1];
			t2d_symbol* dst = &cfg->syms[i];
			memcpy(dst, src, sizeof(t2d_symbol));
		}
		--cfg->sym_count;
	}
}

void MotionTrail::DelAllSymbol()
{
	if (!m_et) {
		return;
	}
	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	if (cfg->mode_type == T2D_MODE_IMAGE) {
		for (int i = 0; i < cfg->sym_count; ++i) {
			s2::Symbol* sym = (s2::Symbol*)(cfg->syms[i].mode.A.ud);
			sym->RemoveReference();
		}
	}
	cfg->sym_count = 0;
}

t2d_symbol* MotionTrail::GetSymbol(int idx)
{
	if (!m_et) {
		return NULL;
	}
	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_et->GetEmitterCfg()->GetImpl());
	if (idx < 0 || idx >= cfg->sym_count) {
		return NULL;
	} else {
		return &cfg->syms[idx];
	}
}

const s2::TrailEmitterCfg* MotionTrail::GetConfig() 
{ 
	if (m_et) {
		return m_et->GetEmitterCfg();
	} else {
		return NULL;
	}
}

void MotionTrail::Init(const s2::TrailEmitterCfg* cfg)
{
	if (!m_et) {
		m_et = s2::TrailEmitterPool::Instance()->Pop();
	}
	m_et->CreateEmitter(cfg);
	m_et->Start();
}

}