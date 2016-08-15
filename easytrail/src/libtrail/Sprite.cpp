#include "Sprite.h"
#include "Symbol.h"
#include "TrailNode.h"

#include <mt_2d.h>
#include <sprite2/RenderParams.h>

#include <assert.h>

namespace etrail
{

Sprite::Sprite()
	: m_et(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: m_et(NULL)
{
}

Sprite::Sprite(Symbol* symbol)
	: ee::Sprite(symbol)
	, m_et(NULL)
{
	const t2d_emitter_cfg* cfg = symbol->GetEmitterCfg();
	if (cfg) {
		m_et = t2d_emitter_create(cfg);
		t2d_emitter_start(m_et);
	}
}

bool Sprite::Update(const s2::RenderParams& params, float dt)
{
	TrailNode::Instance()->UpdateTime();
	
	float time = TrailNode::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	dt = time - m_et->time;
	sm::vec2 pos = params.mt * GetPosition();
	t2d_emitter_update(m_et, dt, (sm_vec2*)(&pos));
	m_et->time = time;

	return true;
}

void Sprite::Draw(const s2::RenderParams& params) const
{
	if (m_et) {
		m_rp.mat = params.mt;
		m_rp.ct = params.color;
		t2d_emitter_draw(m_et, &m_rp);
	}
}

ee::Sprite* Sprite::Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}