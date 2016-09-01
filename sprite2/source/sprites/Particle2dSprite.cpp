#include "Particle2dSprite.h"
#include "Particle2dSymbol.h"
#include "S2_Symbol.h"
#include "RenderParams.h"

#include <ps_2d.h>

#include <stddef.h>
#include <assert.h>

namespace s2
{

Particle2dSprite::Particle2dSprite() 
	: m_et(NULL)
{
}

Particle2dSprite::Particle2dSprite(const Particle2dSprite& spr)
	: m_et(NULL)
{
	if (spr.m_et) {
		m_et = p2d_emitter_create(spr.m_et->cfg);
	}
}

Particle2dSprite& Particle2dSprite::operator = (const Particle2dSprite& spr)
{
	m_et = NULL;
	if (spr.m_et) {
		m_et = p2d_emitter_create(spr.m_et->cfg);
	}
	return *this;
}

Particle2dSprite::Particle2dSprite(Symbol* sym) 
	: Sprite(sym) 
	, m_et(NULL)
{
	const p2d_emitter_cfg* cfg = VI_DOWNCASTING<Particle2dSymbol*>(sym)->GetEmitterCfg();
	if (cfg) {
		m_et = p2d_emitter_create(cfg);
		p2d_emitter_start(m_et);
	}
}

Particle2dSprite::~Particle2dSprite()
{
	if (m_et) {
		p2d_emitter_release(m_et);
	}
}

Particle2dSprite* Particle2dSprite::Clone() const
{
	return new Particle2dSprite(*this);
}

bool Particle2dSprite::Update(const RenderParams& params)
{
	if (!m_et) {
		return false;
	}

	float time = Particle2d::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	float mt[6];
	mt[0] = m_mat.x[0];
	mt[1] = m_mat.x[1];
	mt[2] = m_mat.x[4];
	mt[3] = m_mat.x[5];
	mt[4] = m_mat.x[12];
	mt[5] = m_mat.x[13];	

	float dt = time - m_et->time;
	p2d_emitter_update(m_et, dt, mt);
	m_et->time = time;

	return true;
}

void Particle2dSprite::Draw(const RenderParams& params) const
{
	if (!m_et) {
		return;
	}
	if (m_et->local_mode_draw) {
		p2d_emitter_draw(m_et, &params.mt);
	} else {
		p2d_emitter_draw(m_et, NULL);
	}
}

bool Particle2dSprite::GetLoop() const
{
	if (m_et) {
		return m_et->loop;
	} else {
		return false;
	}
}

void Particle2dSprite::SetLoop(bool loop)
{
	if (!m_et || m_et->loop == loop) {
		return;
	}

	m_et->loop = loop;
	if (m_et->loop) {
		p2d_emitter_start(m_et);
	} else {
		p2d_emitter_pause(m_et);
	}
}

bool Particle2dSprite::GetLocalModeDraw() const
{
	if (m_et) {
		return m_et->local_mode_draw;
	} else {
		return false;
	}
}

void Particle2dSprite::SetLocalModeDraw(bool local)
{
	if (m_et) {
		m_et->local_mode_draw = local;
	}
}

}