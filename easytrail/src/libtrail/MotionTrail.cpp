#include "MotionTrail.h"
#include "mt_config.h"
#include "TrailNode.h"

#include <mt_2d.h>

#include <assert.h>
#include <string>

namespace etrail
{

MotionTrail::MotionTrail(t2d_emitter_cfg* cfg)
{
	TrailNode::Instance();

	Init(cfg);
}

void MotionTrail::SetValue(int key, const ee::UICallback::Data& data)
{
	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_spr->cfg);

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
	switch (key)
	{
	case MT_COUNT:
		data.val0 = static_cast<float>(m_spr->cfg->count);
		break;
	case MT_LIFETIME_BEGIN:
		if (m_spr->cfg->life_begin == FLT_MAX) {
			data.val0 = -1;
		} else {
			data.val0 = m_spr->cfg->life_begin * 1000;
		}
		break;
	case MT_LIFETIME_OFFSET:
		data.val0 = m_spr->cfg->life_offset * 1000;
		break;
	case MT_FADEOUT_TIME:
		data.val0 = m_spr->cfg->fadeout_time * 1000;
		break;
	}
}

void MotionTrail::Draw(const sm::mat4& mt) const
{
	m_rp.mat = const_cast<sm::mat4&>(mt);
	t2d_emitter_draw(m_spr, &m_rp);
}

bool MotionTrail::Update(const sm::vec2& pos)
{
	float time = TrailNode::Instance()->GetTime();
	assert(m_spr->time <= time);
	if (m_spr->time == time) {
		return false;
	}

	float dt = time - m_spr->time;
	t2d_emitter_update(m_spr, dt, (sm_vec2*)(&pos));
	m_spr->time = time;

	return true;
}

void MotionTrail::Start()
{
	t2d_emitter_start(m_spr);
}

void MotionTrail::Stop()
{
	t2d_emitter_stop(m_spr);
}

void MotionTrail::Clear()
{
	t2d_emitter_clear(m_spr);
}

t2d_symbol* MotionTrail::AddSymbol(ee::Symbol* symbol)
{
	assert(m_spr->cfg->symbol_count < MAX_COMPONENTS);

	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_spr->cfg);
	t2d_symbol& comp = m_spr->cfg->symbols[cfg->symbol_count++];
	memset(&comp, 0, SIZEOF_T2D_SYMBOL);

	comp.col_begin.r = comp.col_begin.g = comp.col_begin.b = comp.col_begin.a = 255;
	comp.col_end.r = comp.col_end.g = comp.col_end.b = comp.col_end.a = 255;
	if (cfg->mode_type == T2D_MODE_IMAGE) {
		memset(&comp.mode.A.add_col_begin, 0, sizeof(comp.mode.A.add_col_begin));
		memset(&comp.mode.A.add_col_end, 0, sizeof(comp.mode.A.add_col_end));
		comp.mode.A.scale_begin = comp.mode.A.scale_end = 1;
		comp.mode.A.ud = symbol;
	} else {
		comp.mode.B.size = 1;
		comp.mode.B.acuity = 0;
	}

	return &comp;
}

void MotionTrail::DelSymbol(int idx)
{
	if (idx < 0 || idx >= m_spr->cfg->symbol_count) {
		return;
	}

	t2d_emitter_cfg* cfg = const_cast<t2d_emitter_cfg*>(m_spr->cfg);
	if (cfg->symbol_count == 1) {
		cfg->symbol_count = 0;
	} else {
		for (int i = idx; i < cfg->symbol_count - 1; ++i) {
			const t2d_symbol* src = &cfg->symbols[i+1];
			t2d_symbol* dst = &cfg->symbols[i];
			memcpy(dst, src, sizeof(t2d_symbol));
		}
		--cfg->symbol_count;
	}
}

void MotionTrail::DelAllSymbol()
{
	const_cast<t2d_emitter_cfg*>(m_spr->cfg)->symbol_count = 0;
}

t2d_symbol* MotionTrail::GetSymbol(int idx)
{
	if (idx < 0 || idx >= m_spr->cfg->symbol_count) {
		return NULL;
	} else {
		return &m_spr->cfg->symbols[idx];
	}
}

t2d_emitter_cfg* MotionTrail::GetConfig() 
{ 
	return const_cast<t2d_emitter_cfg*>(m_spr->cfg);
}

void MotionTrail::Init(const t2d_emitter_cfg* cfg)
{
	m_spr = t2d_emitter_create(cfg);
	assert(m_spr);

	m_rp.trail = m_spr;
}

}