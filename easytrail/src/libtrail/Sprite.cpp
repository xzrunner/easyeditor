#include "Sprite.h"
#include "TrailNode.h"

#include <ee/SpriteFactory.h>

#include <mt_2d.h>
#include <sprite2/DummySprite.h>
#include <sprite2/RenderParams.h>

#include <assert.h>

namespace etrail
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_et(NULL)
{
	m_core = new s2::DummySprite(this);
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_et(NULL)
{
	m_core = new s2::DummySprite(*static_cast<s2::DummySprite*>(sprite.m_core), this);

	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_et(NULL)
{
	m_core = new s2::DummySprite(this);

	m_symbol->Retain();
	BuildBounding();

	const t2d_emitter_cfg* cfg = m_symbol->GetEmitterCfg();
	if (cfg) {
		m_et = t2d_emitter_create(cfg);
		t2d_emitter_start(m_et);
	}
}

Sprite::~Sprite()
{
	m_core->RemoveReference();

	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	ee::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
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

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Draw(const s2::RenderParams& params) const
{
	if (m_et) {
		m_rp.mat = params.mt;
		m_rp.ct = params.color;
		t2d_emitter_draw(m_et, &m_rp);
	}
}

}