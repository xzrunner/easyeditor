#include "dev_config.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "Exception.h"
#include "BBFactory.h"
#include "BoundingBox.h"
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
	: m_core(NULL)
	, m_mirror(false, false)
	, m_perspective(0, 0)
	, m_bounding(NULL)
	, m_clip(false)
	, m_is_anchor(false)
	, m_editable(true)
	, m_observer(NULL)
{
	m_offset.MakeInvalid();
}

Sprite::Sprite(const Sprite& sprite)
	: m_core(NULL)
	, m_offset(sprite.m_offset)
	, m_mirror(sprite.m_mirror)
	, m_perspective(sprite.m_perspective)
	, m_bounding(sprite.m_bounding->Clone())
	, m_name(sprite.m_name)
	, m_tag(sprite.m_tag)
	, m_clip(sprite.m_clip)
	, m_is_anchor(sprite.m_is_anchor)
	, m_editable(sprite.m_editable)
	, m_observer(NULL)
{
}

Sprite::~Sprite()
{
	delete m_bounding;
	SpriteFactory::Instance()->Remove(this);
}

Sprite* Sprite::Clone() const
{
	return NULL;
}

void Sprite::ClearUserData(bool deletePtr)
{
	delete m_ud, m_ud = NULL;
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	SpriteIO::Load(val, this);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	SpriteIO::Store(val, this);
}

void Sprite::BuildBounding()
{
	assert(m_core);

	if (!m_bounding) {
		m_bounding = BBFactory::CreateBB(e_obb);
	}
	const Symbol& symbol = GetSymbol();
	sm::rect rect(symbol.GetSize(this));
	if (!rect.IsValid()) {
		return;
	}

	if (!m_offset.IsValid()) {
		m_offset = rect.Center();
	}
	rect.Scale(m_core->GetScale().x, m_core->GetScale().y);
//	rect.Shear(m_core->Shear().x, m_core->Shear().y);
	m_bounding->InitFromRect(rect);
	m_bounding->SetMirror(m_mirror.x, m_mirror.y);
	m_bounding->SetTransform(m_core->GetPosition(), m_offset, m_core->GetAngle());
}

PropertySetting* Sprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

/************************************************************************/
/* geometry                                                             */
/************************************************************************/

void Sprite::SetTransform(const sm::vec2& position, float angle)
{
	assert(m_core);
	if (m_core->GetPosition() != position) Translate(position - m_core->GetPosition());
	if (m_core->GetAngle() != angle) Rotate(angle - m_core->GetAngle());
}

const sm::vec2& Sprite::GetPosition() const 
{ 
	assert(m_core);
	return m_core->GetPosition(); 
}

float Sprite::GetAngle() const 
{
	assert(m_core);
	return m_core->GetAngle(); 
}

void Sprite::Translate(const sm::vec2& offset)
{
	assert(m_core);

#ifdef OPEN_SCREEN_CACHE
	bool find = SpatialPartition::Instance()->Remove(this);
	if (find) {
		SpriteRenderer::InvalidRect(this);
	}
#endif // OPEN_SCREEN_CACHE

	if (m_observer)
		m_observer->Translate(this, offset);

	m_core->SetPosition(m_core->GetPosition() + offset);
	if (m_bounding) {
		m_bounding->SetTransform(m_core->GetPosition(), m_offset, m_core->GetAngle());
	}

#ifdef OPEN_SCREEN_CACHE
	if (find) {
		SpriteRenderer::InvalidRect(this);
		SpatialPartition::Instance()->Insert(this);
	}
#endif // OPEN_SCREEN_CACHE
}

void Sprite::Rotate(float delta)
{
	assert(m_core);

#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Remove(this);
#endif // OPEN_SCREEN_CACHE

	if (m_observer)
		m_observer->Rotate(this, delta);

	m_core->SetAngle(m_core->GetAngle() + delta);

	if (m_bounding) {
		m_bounding->SetTransform(m_core->GetPosition(), m_offset, m_core->GetAngle());
	}

#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Insert(this);
#endif // OPEN_SCREEN_CACHE
}

const sm::vec2& Sprite::GetScale() const 
{ 
	assert(m_core);
	return m_core->GetScale(); 
}

void Sprite::SetScale(const sm::vec2& scale)
{
	assert(m_core);
	const sm::vec2& old_scale = m_core->GetScale();
	if (old_scale.x != 0 && old_scale.y != 0) {
		sm::vec2 dscale;
		dscale.x = scale.x / m_core->GetScale().x;
		dscale.y = scale.y / m_core->GetScale().y;

		sm::vec2 old_offset = m_offset;
		sm::vec2 new_offset(m_offset.x * dscale.x, m_offset.y * dscale.y);
		m_offset = new_offset;

		Translate(old_offset - new_offset);
	}

	m_core->SetScale(scale);
	BuildBounding();
}

const sm::vec2& Sprite::GetShear() const
{
	assert(m_core);
	return m_core->GetShear();
}

void Sprite::SetShear(const sm::vec2& shear)
{
	assert(m_core);
	sm::mat4 mat_old, mat_new;
	mat_old.Shear(m_core->GetShear().x, m_core->GetShear().y);
	mat_new.Shear(shear.x, shear.y);

	sm::vec2 offset = Math2D::TransVector(m_offset, mat_new) - Math2D::TransVector(m_offset, mat_old);

	m_offset += offset;
	Translate(-offset);

	m_core->SetShear(shear);
	BuildBounding();
}

void Sprite::SetOffset(const sm::vec2& offset) 
{
	assert(m_core);

	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = GetCenter();
	m_offset = offset;
	sm::vec2 new_center = GetCenter();
	m_core->SetPosition(m_core->GetPosition() + old_center - new_center);

	if (m_bounding) {
		m_bounding->SetTransform(m_core->GetPosition(), m_offset, m_core->GetAngle());
	}
}

void Sprite::SetMirror(bool x_mirror, bool y_mirror) 
{ 
	assert(m_core);

	bool x_dirty = (x_mirror != m_mirror.x),
		y_dirty = (y_mirror != m_mirror.y);

	if (x_dirty) {
		m_offset.x = -m_offset.x;
	}
	if (y_dirty) {
		m_offset.y = -m_offset.y;
	}

	m_mirror.x = x_mirror;
	m_mirror.y = y_mirror;
	if (m_bounding) {
		m_bounding->SetMirror(x_dirty, y_dirty);
		m_bounding->SetTransform(m_core->GetPosition(), m_offset, m_core->GetAngle());
	}
}

bool Sprite::IsContain(const sm::vec2& pos) const
{
	return m_bounding ? m_bounding->IsContain(pos) : false;
}

bool Sprite::IsIntersect(const sm::rect& rect) const
{
	return m_bounding ? m_bounding->IsIntersect(rect) : false;
}

sm::vec2 Sprite::GetCenter() const
{
	assert(m_core);
	sm::vec2 center_offset = Math2D::RotateVector(-m_offset, m_core->GetAngle()) + m_offset;
	sm::vec2 center = m_core->GetPosition() + center_offset;
	return center;
}

sm::rect Sprite::GetRect() const
{
	std::vector<sm::vec2> bound;
	GetBounding()->GetBoundPos(bound);
	sm::rect rect;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		rect.Combine(bound[i]);
	}
	return rect;
}

//void Sprite::SetBounding(BoundingBox* bb)
//{
//	if (m_bounding == bb) {
//		return;
//	}
//
//	if (m_bounding) {
//		delete m_bounding;
//	}
//	m_bounding = bb;
//}

sm::mat4 Sprite::GetTransMatrix() const
{
	assert(m_core);
	const float x_scale = m_mirror.x ? -m_core->GetScale().x : m_core->GetScale().x,
		y_scale = m_mirror.y ? -m_core->GetScale().y : m_core->GetScale().y;

	sm::vec2 center = GetCenter();
	sm::mat4 mt;
	mt.SetTransformation(center.x, center.y, m_core->GetAngle(), 
		x_scale, y_scale, 0, 0, m_core->GetShear().x, m_core->GetShear().y);
	return mt;
}

sm::mat4 Sprite::GetTransInvMatrix() const
{
	assert(m_core);
	sm::mat4 mat;
	mat.RotateZ(-m_core->GetAngle());
	mat.Shear(-m_core->GetShear().x, -m_core->GetShear().y);
	mat.Translate(-m_core->GetPosition().x/m_core->GetScale().x, -m_core->GetPosition().y/m_core->GetScale().y, 0);
	mat.Scale(1/m_core->GetScale().x, 1/m_core->GetScale().y, 1);
	return mat;
}

/************************************************************************/
/* render                                                               */
/************************************************************************/

const s2::RenderColor& Sprite::GetColor() const 
{ 
	assert(m_core);
	return m_core->Color(); 
}

s2::RenderColor& Sprite::GetColor() 
{ 
	assert(m_core);
	return m_core->Color(); 
}

const s2::RenderShader& Sprite::GetShader() const 
{ 
	assert(m_core);
	return m_core->Shader();
}

s2::RenderShader& Sprite::GetShader() 
{ 
	assert(m_core);
	return m_core->Shader();
}

const s2::RenderCamera& Sprite::GetCamera() const 
{ 
	assert(m_core);
	return m_core->Camera();
}

s2::RenderCamera& Sprite::GetCamera()
{ 
	assert(m_core);
	return m_core->Camera();
}

bool Sprite::IsVisible() const 
{ 
	return m_core ? m_core->IsVisible() : false;
}

void Sprite::SetVisible(bool visible) 
{ 
	if (m_core) {
		m_core->SetVisible(visible);
	}
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
		return s0->GetSymbol().GetFilepath() < s1->GetSymbol().GetFilepath();
	case e_x:
		return s0->GetPosition().x < s1->GetPosition().x;
	case e_y:
		return s0->GetPosition().y < s1->GetPosition().y;
	case e_x_invert:
		return s0->GetPosition().x > s1->GetPosition().x;
	case e_y_invert:
		return s0->GetPosition().y > s1->GetPosition().y;
	default:
		return s0->GetSymbol().GetFilepath() < s1->GetSymbol().GetFilepath();
	}
}

}