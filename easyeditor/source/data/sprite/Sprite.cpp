#include "dev_config.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "Exception.h"
#include "SpriteFactory.h"
#include "SpriteObserver.h"
#include "SpritePropertySetting.h"
#include "SpriteIO.h"

#include <sprite2/S2_Sprite.h>

#ifdef OPEN_SCREEN_CACHE
#include "render/SpriteRenderer.h"
#include "render/EE_SP.h"
#endif // OPEN_SCREEN_CACHE

namespace ee
{

Sprite::Sprite()
	: s2::Sprite()
	, m_mirror(false, false)
	, m_perspective(0, 0)
	, m_clip(false)
	, m_anchor(false)
	, m_editable(true)
	, m_observer(NULL)
{
	SpriteFactory::Instance()->Insert(this);
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, m_mirror(spr.m_mirror)
	, m_perspective(spr.m_perspective)
	, m_tag(spr.m_tag)
	, m_clip(spr.m_clip)
	, m_anchor(spr.m_anchor)
	, m_editable(spr.m_editable)
	, m_observer(NULL)
{
	SpriteFactory::Instance()->Insert(this);
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	
	m_mirror = spr.m_mirror;
	m_perspective = spr.m_perspective;

	m_tag = spr.m_tag;
	m_clip = spr.m_clip;
	m_anchor = spr.m_anchor;

	m_editable = spr.m_editable;

	m_observer = NULL;

	SpriteFactory::Instance()->Insert(this);

	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, m_mirror(false, false)
	, m_perspective(0, 0)
	, m_clip(false)
	, m_anchor(false)
	, m_editable(true)
	, m_observer(NULL)
{
	SpriteFactory::Instance()->Insert(this);
}

Sprite::~Sprite()
{
	SpriteFactory::Instance()->Remove(this);
}

void Sprite::SetSymbol(Symbol* sym)
{
	SpriteFactory::Instance()->Remove(this);
	s2::Sprite::SetSymbol(sym);
	SpriteFactory::Instance()->Insert(this);
}

void Sprite::ClearUserData(bool deletePtr)
{
	delete m_ud, m_ud = NULL;
}

void Sprite::Translate(const sm::vec2& offset)
{
#ifdef OPEN_SCREEN_CACHE
	bool find = SpatialPartition::Instance()->Remove(this);
	if (find) {
		SpriteRenderer::InvalidRect(this);
	}
#endif // OPEN_SCREEN_CACHE

	if (m_observer)
		m_observer->Translate(this, offset);

	SetPosition(m_position + offset);

#ifdef OPEN_SCREEN_CACHE
	if (find) {
		SpriteRenderer::InvalidRect(this);
		SpatialPartition::Instance()->Insert(this);
	}
#endif // OPEN_SCREEN_CACHE
}

void Sprite::Rotate(float delta)
{
#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Remove(this);
#endif // OPEN_SCREEN_CACHE

	if (m_observer)
		m_observer->Rotate(this, delta);

	SetAngle(m_angle + delta);

#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Insert(this);
#endif // OPEN_SCREEN_CACHE
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	SpriteIO::Load(val, this);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	SpriteIO::Store(val, this);
}

PropertySetting* Sprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

/************************************************************************/
/* geometry                                                             */
/************************************************************************/

void Sprite::SetMirror(const sm::bvec2& mirror) 
{ 
	bool x_dirty = (mirror.x != m_mirror.x),
		 y_dirty = (mirror.y != m_mirror.y);
	if (!x_dirty && !y_dirty) {
		return;
	}

	m_mirror = mirror;

	sm::vec2 offset = m_offset,
		     scale = m_scale;
	if (x_dirty) {
		m_offset.x = -m_offset.x;
		m_scale.x = -m_scale.x;
	}
	if (y_dirty) {
		m_offset.y = -m_offset.y;
		m_scale.y = -m_scale.y;
	}

	SetOffset(m_offset);
	SetScale(m_scale);
}

//////////////////////////////////////////////////////////////////////////
// class SpriteCmp
//////////////////////////////////////////////////////////////////////////

SpriteCmp::SpriteCmp(Type type /*= e_file*/)
	: m_type(type)
{
}

bool SpriteCmp::operator() (const Sprite* s0, const Sprite* s1) const
{
	switch (m_type)
	{
	case e_file:
		return dynamic_cast<const ee::Symbol*>(s0->GetSymbol())->GetFilepath()
			 < dynamic_cast<const ee::Symbol*>(s1->GetSymbol())->GetFilepath();
	case e_x:
		return s0->GetPosition().x < s1->GetPosition().x;
	case e_y:
		return s0->GetPosition().y < s1->GetPosition().y;
	case e_x_invert:
		return s0->GetPosition().x > s1->GetPosition().x;
	case e_y_invert:
		return s0->GetPosition().y > s1->GetPosition().y;
	default:
		return dynamic_cast<const ee::Symbol*>(s0->GetSymbol())->GetFilepath()
			 < dynamic_cast<const ee::Symbol*>(s1->GetSymbol())->GetFilepath();
	}
}

}