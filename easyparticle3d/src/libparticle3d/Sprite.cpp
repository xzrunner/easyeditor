#include "Sprite.h"
#include "ParticleSystem.h"
#include "PS.h"
#include "SpritePropertySetting.h"

#include <ps_3d.h>
#include <ps_3d_buffer.h>

namespace eparticle3d
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_alone(false)
	, m_reuse(false)
{
	m_data.spr = NULL;
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_alone(sprite.m_alone)
	, m_reuse(sprite.m_reuse)
{
	m_symbol->Retain();

	if (sprite.m_alone) {
		p3d_sprite* spr = p3d_buffer_add();
		if (spr) {
			spr->et = p3d_emitter_create(sprite.m_data.spr->et->cfg);
			p3d_emitter_start(spr->et);
		}
		m_data.spr = spr;
		m_data.spr->ud = &m_data.spr;
	} else {
		assert(sprite.m_data.ps);
		p3d_emitter_cfg* cfg = (p3d_emitter_cfg*)(sprite.m_data.ps->GetConfig());
		m_data.ps = new ParticleSystem(cfg, false);
		m_data.ps->Start();
	}
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_alone(false)
	, m_reuse(false)
{
	m_symbol->Retain();
	BuildBounding();

	if (const p3d_emitter_cfg* cfg = symbol->GetEmitterCfg()) {
		m_data.ps = new ParticleSystem(const_cast<p3d_emitter_cfg*>(cfg), false);
		m_data.ps->Start();
	}
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}

	if (!m_alone) {
		delete m_data.ps;
	}
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

	if (!m_alone) {
		m_data.ps->SetDirection(m_dir);
		return m_data.ps->Update(m_mat);
	} else {
		return false;
	}
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

	if (m_alone) {

	} else {
		m_data.ps->SetLoop(p_val["loop"].asBool());
		m_data.ps->SetLocalModeDraw(p_val["local_mode_draw"].asBool());	
	}
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

	p_val["alone"] = m_alone;
	p_val["reuse"] = m_reuse;

	if (m_alone) {
		p_val["loop"] = m_data.spr->et->loop;
		p_val["local_mode_draw"] = m_data.spr->local_mode_draw;
	} else {
		const p3d_emitter* et = m_data.ps->GetEmitter();
		p_val["loop"] = m_data.ps->GetEmitter()->loop;
		p_val["local_mode_draw"] = m_data.ps->IsLocalModeDraw();
	}

	val["particle3d"] = p_val;
}

d2d::IPropertySetting* Sprite::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::Start()
{
	if (m_alone) {
		p3d_emitter_start(m_data.spr->et);
	} else {
		m_data.ps->Start();
	}
}

void Sprite::Draw(const d2d::Matrix& mt) const
{
	if (!m_alone) {
		m_data.ps->Draw(mt);
	}
}

void Sprite::SetMatrix(const d2d::Matrix& mat) 
{ 
	m_mat = mat; 

	if (m_alone) {
		const float* src = mat.getElements();
		float* mt = m_data.spr->mat;
		mt[0] = src[0];
		mt[1] = src[1];
		mt[2] = src[4];
		mt[3] = src[5];
		mt[4] = src[12];
		mt[5] = src[13];	
	}
}

bool Sprite::IsLoop() const
{
	if (m_alone) {
		return m_data.spr->et->loop;
	} else {
		return m_data.ps->GetEmitter()->loop;
	}
}

void Sprite::SetLoop(bool loop)
{
	if (m_alone) {
		if (!m_data.spr->et->loop) {
			p3d_sprite* spr = p3d_buffer_add();
			memcpy(spr, m_data.spr, sizeof(*spr));
			m_data.spr = spr;
			m_data.spr->ud = &m_data.spr;
			p3d_emitter_start(spr->et);
		}
		m_data.spr->et->loop = loop;
	} else {
		m_data.ps->SetLoop(loop);
	}
}

bool Sprite::IsLocalModeDraw() const
{
	if (m_alone) {
		return m_data.spr->local_mode_draw;
	} else {
		return m_data.ps->IsLocalModeDraw();
	}
}

void Sprite::SetLocalModeDraw(bool local)
{
	if (m_alone) {
		m_data.spr->local_mode_draw = local;
	} else {
		m_data.ps->SetLocalModeDraw(local);
	}
}

void Sprite::SetAlone(bool alone) 
{ 
	if (m_alone == alone) {
		return;
	}

	if (m_alone) {
		p3d_emitter_cfg* cfg = m_data.spr->et->cfg;
		ParticleSystem* ps = new ParticleSystem(cfg, false);
		p3d_emitter_clear(m_data.spr->et);
		p3d_buffer_remove(m_data.spr);
		m_data.ps = ps;
		m_data.ps->Start();
	} else {
		p3d_sprite* spr = p3d_buffer_add();
		if (spr) {
			spr->et = p3d_emitter_create(m_data.ps->GetEmitter()->cfg);
			p3d_emitter_start(spr->et);
		}
		delete m_data.ps;
		m_data.spr = spr;
		m_data.spr->ud = &m_data.spr;
	}

 	m_alone = alone; 
}

void Sprite::SetReuse(bool reuse)
{
	m_reuse = reuse;
}

}