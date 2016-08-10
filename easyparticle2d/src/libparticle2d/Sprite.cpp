#include "Sprite.h"
#include "PSNode.h"
#include "ParticleSystem.h"
#include "PropertySetting.h"

#include <ee/Sprite.h>
#include <ee/SpriteFactory.h>

#include <ps_2d.h>
#include <sprite2/Particle2dSprite.h>

namespace eparticle2d
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_ps(NULL)
{
	m_core = new s2::Particle2dSprite(this);
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_ps(NULL)
{
	m_core = new s2::Particle2dSprite(*static_cast<s2::Particle2dSprite*>(sprite.m_core), this);

	if (m_symbol) {
		m_symbol->Retain();
	}

	if (sprite.m_ps) {
		p2d_emitter_cfg* cfg = (p2d_emitter_cfg*)(sprite.m_ps->GetConfig());
		m_ps = new ParticleSystem(cfg);
		m_ps->Start();
	}
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_ps(NULL)
{
	m_core = new s2::Particle2dSprite(this);

	if (m_symbol) {
		m_symbol->Retain();
	}
	BuildBounding();

	if (const p2d_emitter_cfg* cfg = symbol->GetEmitterCfg()) {
		m_ps = new ParticleSystem(const_cast<p2d_emitter_cfg*>(cfg));
		m_ps->Start();
	}
}

Sprite::~Sprite()
{
	m_core->RemoveReference();

	if (m_symbol) {
		m_symbol->Release();
	}

	delete m_ps;
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	ee::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

bool Sprite::Update(const s2::RenderParams& params, float dt)
{
	PSNode::Instance()->UpdateTime();
	return m_ps->Update(m_mat);
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& p_val = val["particle2d"];
	m_ps->SetLoop(p_val["loop"].asBool());
	m_ps->SetLocalModeDraw(p_val["local_mode_draw"].asBool());
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value p_val;
	const p2d_emitter* et = m_ps->GetEmitter();
	p_val["loop"] = et->loop;
	p_val["local_mode_draw"] = et->local_mode_draw;

	val["particle2d"] = p_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

void Sprite::Draw(const sm::mat4& mt) const
{
	m_ps->Draw(mt);
}

bool Sprite::GetLoop() const
{
	return m_ps->GetEmitter()->loop;
}

void Sprite::SetLoop(bool loop)
{
	m_ps->SetLoop(loop);
}

bool Sprite::GetLocalModeDraw() const
{
	return m_ps->GetEmitter()->local_mode_draw;
}

void Sprite::SetLocalModeDraw(bool local)
{
	m_ps->SetLocalModeDraw(local);
}

}