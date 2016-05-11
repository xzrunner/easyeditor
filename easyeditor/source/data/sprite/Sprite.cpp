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

#ifdef OPEN_SCREEN_CACHE
#include "render/SpriteRenderer.h"
#include "render/EE_SP.h"
#endif // OPEN_SCREEN_CACHE

namespace ee
{

Sprite::Sprite()
	: m_observer(NULL)
	, m_offset_valid(false)
{
	clip = false;

	visiable = editable = true;

	m_mirror.Set(false, false);
	m_perspective.Set(0, 0);
	m_bounding = NULL;

	m_is_anchor = false;
}

Sprite::Sprite(const Sprite& sprite)
	: m_observer(NULL)
	, m_offset_valid(sprite.m_offset_valid)
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
	// info
	name = val["name"].asString();
	tag = val["tag"].asString();
	clip = val["clip"].asBool();

	// trans
	SpriteIO::LoadColor(val, m_color);
	SpriteIO::LoadShader(val, m_shader);
	SpriteIO::LoadCamera(val, m_camera);

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
	val["name"] = name;
	val["tag"] = tag;
	val["clip"] = clip;

	SpriteIO::StoreColor(val, m_color);
	SpriteIO::StoreShader(val, m_shader);
	SpriteIO::StoreCamera(val, m_camera);

	val["position"]["x"] = m_position.x;
	val["position"]["y"] = m_position.y;

	val["angle"] = m_angle;

	val["x scale"] = m_scale.x;
	val["y scale"] = m_scale.y;

	val["x shear"] = m_shear.x;
	val["y shear"] = m_shear.y;

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
	rect.Scale(m_scale.x, m_scale.y);
	rect.Shear(m_shear.x, m_shear.y);
	m_bounding->InitFromRect(rect);
	m_bounding->SetTransform(m_position, m_offset, m_angle);
}

PropertySetting* Sprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::SetTransform(const sm::vec2& position, float angle)
{
	if (m_position != position) Translate(position - m_position);
	if (m_angle != angle) Rotate(angle - m_angle);
}

void Sprite::SetScale(const sm::vec2& scale)
{
	sm::vec2 dscale;
	dscale.x = scale.x / m_scale.x;
	dscale.y = scale.y / m_scale.y;

	sm::vec2 old_offset = m_offset;
	sm::vec2 new_offset(m_offset.x * dscale.x, m_offset.y * dscale.y);
	m_offset = new_offset;
	m_offset_valid = true;

	Translate(old_offset - new_offset);

	//////////////////////////////////////////////////////////////////////////

// 	sm::mat4 mat_old, mat_new;
// 	mat_old.scale(m_scale.x, m_scale.y);
// 	mat_new.scale(xScale, yScale);
// 
// 	sm::vec2 offset = Math2D::TransVector(m_offset, mat_new) - Math2D::TransVector(m_offset, mat_old);
// 
// 	m_offset += offset;
// 	translate(-offset);

	//////////////////////////////////////////////////////////////////////////

	m_scale = scale;
	BuildBounding();
 }

void Sprite::SetShear(const sm::vec2& shear)
{
	sm::mat4 mat_old, mat_new;
	mat_old.Shear(m_shear.x, m_shear.y);
	mat_new.Shear(shear.x, shear.y);

	sm::vec2 offset = Math2D::TransVector(m_offset, mat_new) - Math2D::TransVector(m_offset, mat_old);

	m_offset += offset;
	Translate(-offset);

	m_shear = shear;
	BuildBounding();
}

void Sprite::SetOffset(const sm::vec2& offset) 
{
	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = GetCenter();
	m_offset = offset;
	m_offset_valid = true;
	sm::vec2 new_center = GetCenter();
	m_position += (old_center - new_center);

	if (m_bounding) {
		m_bounding->SetTransform(m_position, m_offset, m_angle);
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
#ifdef OPEN_SCREEN_CACHE
	bool find = SpatialPartition::Instance()->Remove(this);
	if (find) {
		SpriteRenderer::InvalidRect(this);
	}
#endif // OPEN_SCREEN_CACHE

	if (m_observer)
		m_observer->Translate(this, offset);

	m_position += offset;
	if (m_bounding) {
		m_bounding->SetTransform(m_position, m_offset, m_angle);
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
#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Remove(this);
#endif // OPEN_SCREEN_CACHE

	if (m_observer)
		m_observer->Rotate(this, delta);

	m_angle += delta;

	if (m_bounding) {
		m_bounding->SetTransform(m_position, m_offset, m_angle);
	}

#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Insert(this);
#endif // OPEN_SCREEN_CACHE
}

void Sprite::SetMirror(bool x_mirror, bool y_mirror) 
{ 
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
		m_bounding->SetTransform(m_position, m_offset, m_angle);
	}
}

sm::vec2 Sprite::GetCenter() const
{
	sm::vec2 center_offset = Math2D::RotateVector(-m_offset, m_angle) + m_offset;
	sm::vec2 center = m_position + center_offset;
	return center;
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
	const float x_scale = m_mirror.x ? -m_scale.x : m_scale.x,
		y_scale = m_mirror.y ? -m_scale.y : m_scale.y;

	sm::vec2 center = GetCenter();
	mt.SetTransformation(center.x, center.y, m_angle, 
		x_scale, y_scale, 0, 0, m_shear.x, m_shear.y);
}

sm::mat4 Sprite::GetTransInvMatrix() const
{
	sm::mat4 mat;
	mat.RotateZ(-m_angle);
	mat.Shear(-m_shear.x, -m_shear.y);
	mat.Translate(-m_position.x/m_scale.x, -m_position.y/m_scale.y, 0);
	mat.Scale(1/m_scale.x, 1/m_scale.y, 1);
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