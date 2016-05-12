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

#include <sprite2/Sprite.h>

#ifdef OPEN_SCREEN_CACHE
#include "render/SpriteRenderer.h"
#include "render/EE_SP.h"
#endif // OPEN_SCREEN_CACHE

namespace ee
{

Sprite::Sprite()
	: m_core(NULL)
	, m_offset_valid(false)
	, m_observer(NULL)
{
	clip = false;

	visiable = editable = true;

	m_mirror.Set(false, false);
	m_perspective.Set(0, 0);
	m_bounding = NULL;

	m_is_anchor = false;
}

Sprite::Sprite(const Sprite& sprite)
	: m_core(NULL)
	, m_offset_valid(sprite.m_offset_valid)
	, m_observer(NULL)
{
	name = sprite.name;
	tag = sprite.tag;
	clip = sprite.clip;

	visiable = sprite.visiable;
	editable = sprite.editable;

	m_offset = sprite.m_offset;
	m_mirror = sprite.m_mirror;
	m_perspective = sprite.m_perspective;
	m_bounding = sprite.m_bounding->Clone();

	m_is_anchor = sprite.m_is_anchor;
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

void Sprite::Load(const Json::Value& val)
{
	assert(m_core);

	// info
	name = val["name"].asString();
	tag = val["tag"].asString();
	clip = val["clip"].asBool();

	// trans
	SpriteIO::LoadColor(val, m_core->Color());
	SpriteIO::LoadShader(val, m_core->Shader());
	SpriteIO::LoadCamera(val, m_core->Camera());

	// scale
	sm::vec2 scale;
	if (val["scale"].isNull())
	{
		scale.x = static_cast<float>(val["x scale"].asDouble());
		scale.y = static_cast<float>(val["y scale"].asDouble());
	}
	else
	{
		scale.x = scale.y = static_cast<float>(val["scale"].asDouble());
	}
	SetScale(scale);

	// shear
	sm::vec2 shear;
	if (!val["x shear"].isNull())
	{
		shear.x = static_cast<float>(val["x shear"].asDouble());
		shear.y = static_cast<float>(val["y shear"].asDouble());
	}
	else
	{
		shear.Set(0, 0);
	}
	SetShear(shear);

	// mirror
	bool mx = val["x mirror"].asBool();
	bool my = val["y mirror"].asBool();
	SetMirror(mx, my);

	// perspective
	if (!val["x perspective"].isNull())
	{
		sm::vec2 persp;
		persp.x = static_cast<float>(val["x perspective"].asDouble());
		persp.y = static_cast<float>(val["y perspective"].asDouble());
		SetPerspective(persp);
	}

	// offset
	float ox, oy;
	if (!val["x offset"].isNull())
	{
		ox = static_cast<float>(val["x offset"].asDouble());
		oy = static_cast<float>(val["y offset"].asDouble());
	}
	else
	{
		ox = oy = 0;
	}
	SetOffset(sm::vec2(ox, oy));

	// translate
	float x = static_cast<float>(val["position"]["x"].asDouble());
	float y = static_cast<float>(val["position"]["y"].asDouble());
	if (fabs(x) >= FLT_MAX || fabs(y) >= FLT_MAX) {
		throw Exception("sprite pos err, filepath: %s, name: %s", val["filepath"].asString().c_str(), name.c_str());
	}

	// rotate
	float angle = static_cast<float>(val["angle"].asDouble());
	SetTransform(sm::vec2(x, y), angle);

	// anchor
	m_is_anchor = val["anchor"].asBool();
}

void Sprite::Store(Json::Value& val) const
{
	assert(m_core);

	val["name"] = name;
	val["tag"] = tag;
	val["clip"] = clip;

	SpriteIO::StoreColor(val, m_core->Color());
	SpriteIO::StoreShader(val, m_core->Shader());
	SpriteIO::StoreCamera(val, m_core->Camera());

	val["position"]["x"] = m_core->Position().x;
	val["position"]["y"] = m_core->Position().y;

	val["angle"] = m_core->Angle();

	val["x scale"] = m_core->Scale().x;
	val["y scale"] = m_core->Scale().y;

	val["x shear"] = m_core->Shear().x;
	val["y shear"] = m_core->Shear().y;

	val["x mirror"] = m_mirror.x;
	val["y mirror"] = m_mirror.y;

	val["x offset"] = m_offset.x;
	val["y offset"] = m_offset.y;

	val["x perspective"] = m_perspective.x;
	val["y perspective"] = m_perspective.y;

	val["anchor"] = m_is_anchor;
}

void Sprite::BuildBounding()
{
	assert(m_core);

	if (!m_bounding) {
		m_bounding = BBFactory::CreateBB(e_obb);
	}
	const Symbol& symbol = GetSymbol();
	Rect rect(symbol.GetSize(this));
	if (!rect.IsValid()) {
		return;
	}

	if (!m_offset_valid) {
		m_offset.Set(rect.CenterX(), rect.CenterY());
	}
	rect.Scale(m_core->Scale().x, m_core->Scale().y);
	rect.Shear(m_core->Shear().x, m_core->Shear().y);
	m_bounding->InitFromRect(rect);
	m_bounding->SetTransform(m_core->Position(), m_offset, m_core->Angle());
}

PropertySetting* Sprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::SetTransform(const sm::vec2& position, float angle)
{
	assert(m_core);
	if (m_core->Position() != position) Translate(position - m_core->Position());
	if (m_core->Angle() != angle) Rotate(angle - m_core->Angle());
}

const sm::vec2& Sprite::GetPosition() const 
{ 
	assert(m_core);
	return m_core->Position(); 
}

float Sprite::GetAngle() const 
{
	assert(m_core);
	return m_core->Angle(); 
}

const sm::vec2& Sprite::GetScale() const 
{ 
	assert(m_core);
	return m_core->Scale(); 
}

void Sprite::SetScale(const sm::vec2& scale)
{
	assert(m_core);

	sm::vec2 dscale;
	dscale.x = scale.x / m_core->Scale().x;
	dscale.y = scale.y / m_core->Scale().y;

	sm::vec2 old_offset = m_offset;
	sm::vec2 new_offset(m_offset.x * dscale.x, m_offset.y * dscale.y);
	m_offset = new_offset;
	m_offset_valid = true;

	Translate(old_offset - new_offset);

	//////////////////////////////////////////////////////////////////////////

// 	sm::mat4 mat_old, mat_new;
// 	mat_old.scale(m_impl->Scale().x, m_impl->Scale().y);
// 	mat_new.scale(xScale, yScale);
// 
// 	sm::vec2 offset = Math2D::TransVector(m_offset, mat_new) - Math2D::TransVector(m_offset, mat_old);
// 
// 	m_offset += offset;
// 	translate(-offset);

	//////////////////////////////////////////////////////////////////////////

	m_core->Scale() = scale;
	BuildBounding();
}

const sm::vec2& Sprite::GetShear() const
{
	assert(m_core);
	return m_core->Shear();
}

void Sprite::SetShear(const sm::vec2& shear)
{
	assert(m_core);
	sm::mat4 mat_old, mat_new;
	mat_old.Shear(m_core->Shear().x, m_core->Shear().y);
	mat_new.Shear(shear.x, shear.y);

	sm::vec2 offset = Math2D::TransVector(m_offset, mat_new) - Math2D::TransVector(m_offset, mat_old);

	m_offset += offset;
	Translate(-offset);

	m_core->Shear() = shear;
	BuildBounding();
}

void Sprite::SetOffset(const sm::vec2& offset) 
{
	assert(m_core);

	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = GetCenter();
	m_offset = offset;
	m_offset_valid = true;
	sm::vec2 new_center = GetCenter();
	m_core->Position() += (old_center - new_center);

	if (m_bounding) {
		m_bounding->SetTransform(m_core->Position(), m_offset, m_core->Angle());
	}
}

bool Sprite::IsContain(const sm::vec2& pos) const
{
	return m_bounding ? m_bounding->IsContain(pos) : false;
}

bool Sprite::IsIntersect(const Rect& rect) const
{
	return m_bounding ? m_bounding->IsIntersect(rect) : false;
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

	m_core->Position() += offset;
	if (m_bounding) {
		m_bounding->SetTransform(m_core->Position(), m_offset, m_core->Angle());
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

	m_core->Angle() += delta;

	if (m_bounding) {
		m_bounding->SetTransform(m_core->Position(), m_offset, m_core->Angle());
	}

#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Insert(this);
#endif // OPEN_SCREEN_CACHE
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
		m_bounding->SetTransform(m_core->Position(), m_offset, m_core->Angle());
	}
}

sm::vec2 Sprite::GetCenter() const
{
	assert(m_core);
	sm::vec2 center_offset = Math2D::RotateVector(-m_offset, m_core->Angle()) + m_offset;
	sm::vec2 center = m_core->Position() + center_offset;
	return center;
}

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

Rect Sprite::GetRect() const
{
	std::vector<sm::vec2> bound;
	GetBounding()->GetBoundPos(bound);
	Rect rect;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		rect.Combine(bound[i]);
	}
	return rect;
}

void Sprite::GetTransMatrix(sm::mat4& mt) const
{
	assert(m_core);
	const float x_scale = m_mirror.x ? -m_core->Scale().x : m_core->Scale().x,
		y_scale = m_mirror.y ? -m_core->Scale().y : m_core->Scale().y;

	sm::vec2 center = GetCenter();
	mt.SetTransformation(center.x, center.y, m_core->Angle(), 
		x_scale, y_scale, 0, 0, m_core->Shear().x, m_core->Shear().y);
}

sm::mat4 Sprite::GetTransInvMatrix() const
{
	assert(m_core);
	sm::mat4 mat;
	mat.RotateZ(-m_core->Angle());
	mat.Shear(-m_core->Shear().x, -m_core->Shear().y);
	mat.Translate(-m_core->Position().x/m_core->Scale().x, -m_core->Position().y/m_core->Scale().y, 0);
	mat.Scale(1/m_core->Scale().x, 1/m_core->Scale().y, 1);
	return mat;
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