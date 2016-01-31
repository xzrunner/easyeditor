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
	, m_spr(NULL)
	, m_alone(false)
	, m_reuse(false)
{
}

Sprite::Sprite(const Sprite& sprite)
	: Sprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_alone(sprite.m_alone)
	, m_reuse(sprite.m_reuse)
{
	m_symbol->Retain();

//	CreatePS();

	m_spr = sprite.m_spr;

	if (m_alone) {
		p3d_buffer_insert(m_spr);
	}
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_alone(false)
	, m_reuse(false)
{
	m_symbol->Retain();
	BuildBounding();

	CreatePS();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}

	if (m_spr) {
		if (!m_alone) {
			p3d_emitter_release(m_spr->et);
		} else {
			if (m_spr->et->loop) {
				p3d_buffer_remove(m_spr);
				p3d_sprite_release(m_spr);
			}
		}
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	ee::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

bool Sprite::Update(int version)
{
	PS::Instance()->UpdateTime();

	if (!m_spr) {
		return true;
	} else if (m_alone) {
		return false;
	} else {
		p3d_emitter* et = m_spr->et;

		float time = PS::Instance()->GetTime();
		assert(et->time <= time);
		if (et->time == time) {
			return false;
		}

		const float* src = m_mat.GetElements();
		float mt[6];
		mt[0] = src[0];
		mt[1] = src[1];
		mt[2] = src[4];
		mt[3] = src[5];
		mt[4] = src[12];
		mt[5] = src[13];	

		float dt = time - et->time;
		p3d_emitter_update(et, dt, mt);
		et->time = time;

		return true;
	}
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	Sprite::Load(val);

	const Json::Value& p_val = val["particle3d"];

	const Json::Value& d_val = p_val["dir"];
	float x = d_val["x"].asDouble(),
		  y = d_val["y"].asDouble(),
		  z = d_val["z"].asDouble(),
		  w = d_val["w"].asDouble();
	m_dir = Quaternion(x, y, z, w);

	if (p_val["alone"].isNull()) {
		m_alone = false;
	} else {
		m_alone = p_val["alone"].asBool();
	}
	if (p_val["reuse"].isNull()) {
		m_reuse = false;
	} else {
		m_reuse = p_val["reuse"].asBool();
	}
	
	m_spr = p3d_sprite_create();
	if (m_alone) {
		p3d_buffer_insert(m_spr);
	}
	if (m_spr) {
		if (m_reuse) {
			m_spr->et = m_symbol->GetEmitter();
		} else {
			m_spr->et = p3d_emitter_create(m_symbol->GetEmitterCfg());
		}
		p3d_emitter_start(m_spr->et);
		m_spr->ptr_self = &m_spr;
		if (p_val["loop"].isNull()) {
			m_spr->et->loop = true;
		} else {
			m_spr->et->loop = p_val["loop"].asBool();
		}
		if (p_val["local_mode_draw"].isNull()) {
			m_spr->local_mode_draw = false;
		} else {
			m_spr->local_mode_draw = p_val["local_mode_draw"].asBool();
		}
	}
}

void Sprite::Store(Json::Value& val) const
{
	Sprite::Store(val);

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

	if (m_spr) {
		p_val["loop"] = m_spr->et->loop;
		p_val["local_mode_draw"] = m_spr->local_mode_draw;
	} else {
		p_val["loop"] = true;
		p_val["local_mode_draw"] = true;
	}

	val["particle3d"] = p_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::Start()
{
	if (m_spr) {
		p3d_emitter_start(m_spr->et);
	}
}

void Sprite::Draw(const ee::Matrix& mt) const
{
	if (!m_alone && m_spr) {
		m_rp.mat = mt;
		m_rp.p3d = m_spr;
		p3d_emitter_draw(m_spr->et, &m_rp);
	}
}

void Sprite::SetMatrix(const ee::Matrix& mat) 
{ 
	m_mat = mat;
	if (m_spr && m_alone) {
		const float* src = mat.GetElements();
		float* mt = m_spr->mat;
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
	if (m_spr) {
		return m_spr->et->loop;
	} else {
		return false;
	}
}

void Sprite::SetLoop(bool loop)
{
	// removed from buffer
	if (!m_spr) {
		CreatePS();
		p3d_buffer_insert(m_spr);
	}

	m_spr->et->loop = loop;
}

bool Sprite::IsLocalModeDraw() const
{
	if (m_spr) {
		return m_spr->local_mode_draw;
	} else {
		return true;
	}
}

void Sprite::SetLocalModeDraw(bool local)
{
	if (m_spr) {
		m_spr->local_mode_draw = local;
	}
}

void Sprite::SetAlone(bool alone) 
{ 
	if (m_alone == alone || !m_spr) {
		return;
	}

	p3d_emitter_clear(m_spr->et);
	const p3d_emitter_cfg* cfg = m_spr->et->cfg;
	p3d_emitter* et = p3d_emitter_create(cfg);
	if (m_alone) {
		p3d_buffer_remove(m_spr);
	} else {
		p3d_buffer_insert(m_spr);
	}
	if (m_spr) {
		m_spr->et = et;
		p3d_emitter_start(m_spr->et);
		m_spr->ptr_self = &m_spr;
	}
	m_alone = alone;
}

void Sprite::SetReuse(bool reuse)
{
	if (m_reuse == reuse || !m_spr) {
		return;
	}

	if (m_reuse) {
		m_spr->et = p3d_emitter_create(m_spr->et->cfg);
	} else {
		p3d_emitter_release(m_spr->et);
		m_spr->et = m_symbol->GetEmitter();
	}
	p3d_emitter_start(m_spr->et);

	m_reuse = reuse;
}

void Sprite::OnActive()
{
	// removed from buffer
	if (!m_spr) {
		CreatePS();
	}

	if (m_alone) {
		p3d_buffer_insert(m_spr);
	}
}

void Sprite::CreatePS()
{
	const p3d_emitter_cfg* cfg = m_symbol->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_spr = p3d_sprite_create();
	m_spr->et = p3d_emitter_create(cfg);
	p3d_emitter_start(m_spr->et);
	m_spr->ptr_self = &m_spr;
}

}