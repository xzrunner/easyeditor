#include "Sprite.h"
#include "Symbol.h"
//#include "ParticleSystem.h"
#include "SpritePropertySetting.h"

#include <ps_3d.h>
#include <sprite2/RenderParams.h>
#include <sprite2/Particle3dEmitter.h>
#include <s2loader/P3dSprLoader.h>

namespace eparticle3d
{

Sprite::Sprite()
{
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::Particle3dSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& p_val = val["particle3d"];

	const Json::Value& d_val = p_val["dir"];
	float x = d_val["x"].asDouble(),
		  y = d_val["y"].asDouble(),
		  z = d_val["z"].asDouble(),
		  w = d_val["w"].asDouble();
	m_dir = sm::Quaternion(x, y, z, w);

	s2loader::P3dSprLoader loader(*this);
	loader.LoadJson(val, dir.c_str());
}

void Sprite::Store(Json::Value& val, const std::string& _dir) const
{
	ee::Sprite::Store(val);

	Json::Value p_val;

	sm::vec4 dir = m_dir.ToVector();
	Json::Value d_val;
	d_val["x"] = dir.x;
	d_val["y"] = dir.y;
	d_val["z"] = dir.z;
	d_val["w"] = dir.w;
	p_val["dir"] = d_val;

	p_val["loop"] = m_loop;
	p_val["local_mode_draw"] = m_local;
	p_val["alone"] = m_alone;
	p_val["reuse"] = m_reuse;

	p_val["start_radius"] = m_start_radius;

	val["particle3d"] = p_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

void Sprite::OnActive()
{
	m_et.reset();
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}