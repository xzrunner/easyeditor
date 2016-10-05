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
	, m_perspective(0, 0)
	, m_clip(false)
	, m_anchor(false)
	, m_observer(NULL)
{
	SpriteFactory::Instance()->Insert(this);
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, m_perspective(spr.m_perspective)
	, m_tag(spr.m_tag)
	, m_clip(spr.m_clip)
	, m_anchor(spr.m_anchor)
	, m_observer(NULL)
{
	SpriteFactory::Instance()->Insert(this);
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	
	m_perspective = spr.m_perspective;

	m_tag = spr.m_tag;
	m_clip = spr.m_clip;
	m_anchor = spr.m_anchor;

	m_observer = NULL;

	SpriteFactory::Instance()->Insert(this);

	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, m_perspective(0, 0)
	, m_clip(false)
	, m_anchor(false)
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

void Sprite::SetPosition(const sm::vec2& pos)
{
	PartialUpdate pu;
	pu.Begin();

	s2::Sprite::SetPosition(pos);

	if (m_observer) {
		m_observer->OnSetPosition(this, pos);
	}

	pu.End();
}

void Sprite::SetAngle(float angle)
{
	PartialUpdate pu;
	pu.Begin();

	s2::Sprite::SetAngle(angle);

	if (m_observer) {
		m_observer->OnSetAngle(this, angle);
	}

	pu.End();
}

void Sprite::SetScale(const sm::vec2& scale)
{
	PartialUpdate pu;
	pu.Begin();

	s2::Sprite::SetScale(scale);

	if (m_observer) {
		m_observer->OnSetScale(this, scale);
	}

	pu.End();
}

void Sprite::SetShear(const sm::vec2& shear)
{
	PartialUpdate pu;
	pu.Begin();

	s2::Sprite::SetShear(shear);

	if (m_observer) {
		m_observer->OnSetShear(this, shear);
	}

	pu.End();
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	PartialUpdate pu;
	pu.Begin();

	s2::Sprite::SetOffset(offset);

	if (m_observer) {
		m_observer->OnSetOffset(this, offset);
	}

	pu.End();
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::SpriteIO spr_io;
	spr_io.Load(val, this);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::SpriteIO spr_io;
	spr_io.Store(val, this);
}

PropertySetting* Sprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

/************************************************************************/
/* class Sprite::PartialUpdate                                          */
/************************************************************************/

void Sprite::PartialUpdate::Begin()
{
#ifdef OPEN_SCREEN_CACHE
	m_find = SpatialPartition::Instance()->Remove(this);
	if (m_find) {
		SpriteRenderer::InvalidRect(this);
	}
#endif // OPEN_SCREEN_CACHE
}

void Sprite::PartialUpdate::End()
{
#ifdef OPEN_SCREEN_CACHE
	if (m_find) {
		SpriteRenderer::InvalidRect(this);
		SpatialPartition::Instance()->Insert(this);
	}
#endif // OPEN_SCREEN_CACHE
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