#include "Sprite.h"
#include "PS.h"
#include "ParticleSystem.h"
#include "PropertySetting.h"

#include <ps_2d.h>

namespace eparticle2d
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_ps(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_ps(NULL)
{
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
	if (m_symbol) {
		m_symbol->Release();
	}

	delete m_ps;
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

bool Sprite::Update(int version)
{
	PS::Instance()->UpdateTime();
	return m_ps->Update(m_mat);
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	d2d::ISprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	ISprite::Load(val);

	const Json::Value& p_val = val["particle2d"];
	m_ps->SetLoop(p_val["loop"].asBool());
	m_ps->SetLocalModeDraw(p_val["local_mode_draw"].asBool());
}

void Sprite::Store(Json::Value& val) const
{
	ISprite::Store(val);

	Json::Value p_val;
	const p2d_emitter* et = m_ps->GetEmitter();
	p_val["loop"] = et->loop;
	p_val["local_mode_draw"] = et->local_mode_draw;

	val["particle2d"] = p_val;
}

d2d::IPropertySetting* Sprite::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

void Sprite::Draw(const d2d::Matrix& mt) const
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