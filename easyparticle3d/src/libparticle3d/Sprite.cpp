#include "Sprite.h"
#include "ParticleSystem.h"
#include "PS.h"
#include "SpritePropertySetting.h"

#include <ps_3d.h>

namespace eparticle3d
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
	m_symbol->Retain();

	if (sprite.m_ps) {
		p3d_emitter_cfg* cfg = (p3d_emitter_cfg*)(sprite.m_ps->GetConfig());
		m_ps = new ParticleSystem(cfg, false);
		m_ps->Start();
	}
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_ps(NULL)
{
	m_symbol->Retain();
	BuildBounding();

	if (const p3d_emitter_cfg* cfg = symbol->GetEmitterCfg()) {
		m_ps = new ParticleSystem(const_cast<p3d_emitter_cfg*>(cfg), false);
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

	m_ps->SetDirection(m_dir);
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

	const Json::Value& p_val = val["particle3d"];

	const Json::Value& d_val = p_val["dir"];
	float x = d_val["x"].asDouble(),
		  y = d_val["y"].asDouble(),
		  z = d_val["z"].asDouble(),
		  w = d_val["w"].asDouble();
	m_dir = Quaternion(x, y, z, w);

	m_ps->SetLoop(p_val["loop"].asBool());
	m_ps->SetLocalModeDraw(p_val["local_mode_draw"].asBool());
}

void Sprite::Store(Json::Value& val) const
{
	ISprite::Store(val);

	Json::Value p_val;

	vec4 dir = m_dir.ToVector();
	Json::Value d_val;
	d_val["x"] = dir.x;
	d_val["y"] = dir.y;
	d_val["z"] = dir.z;
	d_val["w"] = dir.w;
	p_val["dir"] = d_val;

	const p3d_emitter* et = m_ps->GetEmitter();
	p_val["loop"] = et->loop;
	p_val["local_mode_draw"] = et->local_mode_draw;

	val["particle3d"] = p_val;
}

d2d::IPropertySetting* Sprite::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::Start()
{
	m_ps->Start();
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