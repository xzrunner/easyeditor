#include "TrailSprite.h"
#include "TrailSymbol.h"
#include "RenderParams.h"

#include <mt_2d.h>

#include <assert.h>

namespace s2
{

TrailSprite::TrailSprite()
	: m_et(NULL)
{
}

TrailSprite::TrailSprite(const TrailSprite& spr)
	: Sprite(spr)
	, m_et(NULL)
{
	if (spr.m_et) {
		m_et = t2d_emitter_create(spr.m_et->cfg);
		t2d_emitter_start(m_et);
	}
}

TrailSprite& TrailSprite::operator = (const TrailSprite& spr)
{
	m_et = NULL;
	if (spr.m_et) {
		m_et = t2d_emitter_create(spr.m_et->cfg);
		t2d_emitter_start(m_et);
	}
	return *this;
}

TrailSprite::TrailSprite(Symbol* sym)
	: Sprite(sym)
	, m_et(NULL)
{
	const t2d_emitter_cfg* cfg = VI_DOWNCASTING<TrailSymbol*>(sym)->GetEmitterCfg();
	if (cfg) {
		m_et = t2d_emitter_create(cfg);
		t2d_emitter_start(m_et);
	}
}

TrailSprite::~TrailSprite()
{
	if (m_et) {
		t2d_emitter_release(m_et);
	}
}

TrailSprite* TrailSprite::Clone() const
{
	return new TrailSprite(*this);
}

bool TrailSprite::Update(const RenderParams& params)
{
	float time = Trail::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	float dt = time - m_et->time;
	sm::vec2 pos = params.mt * GetPosition();
	t2d_emitter_update(m_et, dt, (sm_vec2*)(&pos));
	m_et->time = time;

	return true;
}

void TrailSprite::Draw(const RenderParams& params) const
{
	if (m_et) {
		m_rp.mat = params.mt;
		m_rp.ct = params.color;
		t2d_emitter_draw(m_et, &m_rp);
	}
}

}