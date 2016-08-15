#include "Sprite.h"
#include "Symbol.h"
#include "PSNode.h"
#include "ParticleSystem.h"
#include "PropertySetting.h"

#include <ee/Sprite.h>

#include <ps_2d.h>

namespace eparticle2d
{

Sprite::Sprite()
	: m_ps(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_ps(NULL)
{
	if (sprite.m_ps) {
		p2d_emitter_cfg* cfg = (p2d_emitter_cfg*)(sprite.m_ps->GetConfig());
		m_ps = new ParticleSystem(cfg);
		m_ps->Start();
	}
}

Sprite::Sprite(Symbol* symbol)
	: ee::Sprite(symbol)
	, m_ps(NULL)
{
	if (const p2d_emitter_cfg* cfg = symbol->GetEmitterCfg()) {
		m_ps = new ParticleSystem(const_cast<p2d_emitter_cfg*>(cfg));
		m_ps->Start();
	}
}

Sprite::~Sprite()
{
	delete m_ps;
}

bool Sprite::Update(const s2::RenderParams& params, float dt)
{
	PSNode::Instance()->UpdateTime();
	return m_ps->Update(m_mat);
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

ee::Sprite* Sprite::Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}