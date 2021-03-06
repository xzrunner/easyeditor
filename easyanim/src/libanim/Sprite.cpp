#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/std_functor.h>

#include <sprite2/UpdateParams.h>
#include <sprite2/AnimCurr.h>
#include <s2loader/AnimSprLoader.h>

namespace libanim
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::AnimSprite(spr)
	, ee::Sprite(spr)
	, m_static_time(spr.m_static_time)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::AnimSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	m_static_time = spr.m_static_time;
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::AnimSprite(sym)
	, ee::Sprite(sym)
	, m_static_time(-1)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	s2loader::AnimSprLoader loader(*this);
	loader.LoadJson(val, dir.c_str());
	
	const Json::Value& anim_val = val["animation"];
	m_static_time = anim_val["static_time"].asInt();
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value anim_val;

	anim_val["loop"] = m_loop;
	anim_val["interval"] = m_interval;

	anim_val["fps"] = m_fps;

	anim_val["start_random"] = m_start_random;

	anim_val["static_time"] = m_static_time;

	anim_val["active"] = m_state.GetOrigin().IsActive();

	val["animation"] = anim_val;
}

void Sprite::Load(const s2s::NodeSpr* spr)
{
	ee::Sprite::Load(spr);

	s2loader::AnimSprLoader loader(*this);
	loader.LoadSns(spr);
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

void Sprite::SetStaticTime(int static_time)
{
	m_static_time = static_time;
	SetFrame(s2::UpdateParams(), static_time);
}

bool Sprite::IsActive() const 
{ 
	return m_state.GetOrigin().IsActive(); 
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}