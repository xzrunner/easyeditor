#include "dev_config.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "Exception.h"
#include "BBFactory.h"
#include "BoundingBox.h"
#include "SpriteFactory.h"
#include "RenderColor.h"
#include "SpriteObserver.h"
#include "SpritePropertySetting.h"
#include "RenderParams.h"

#ifdef OPEN_SCREEN_CACHE
#include "render/SpriteRenderer.h"
#include "render/EE_SP.h"
#endif // OPEN_SCREEN_CACHE

namespace ee
{

Sprite::Sprite()
	: m_observer(NULL)
{
	clip = false;

	rp = new RenderParams;

	visiable = editable = true;

	m_angle = 0.0f;
	m_offset.SetInvalid();
	m_scale.Set(1, 1);
	m_shear.Set(0, 0);
	m_xMirror = m_yMirror = false;
	m_perspective.Set(0, 0);
	m_bounding = NULL;

	m_is_anchor = false;
}

Sprite::Sprite(const Sprite& sprite)
	: m_observer(NULL)
{
	name = sprite.name;
	tag = sprite.tag;
	clip = sprite.clip;

	rp = new RenderParams(*sprite.rp);

	visiable = sprite.visiable;
	editable = sprite.editable;

	m_pos = sprite.m_pos;
	m_angle = sprite.m_angle;
	m_offset = sprite.m_offset;
	m_scale = sprite.m_scale;
	m_shear = sprite.m_shear;
	m_xMirror = sprite.m_xMirror;
	m_yMirror = sprite.m_yMirror;
	m_perspective = sprite.m_perspective;
	m_bounding = sprite.m_bounding->Clone();

	m_is_anchor = sprite.m_is_anchor;
}

Sprite::~Sprite()
{
	delete rp;
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
	rp->LoadFromFile(val);

	// scale
	Vector scale;
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
	Vector shear;
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
		Vector persp;
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
	SetOffset(Vector(ox, oy));

	// translate
	float x = static_cast<float>(val["position"]["x"].asDouble());
	float y = static_cast<float>(val["position"]["y"].asDouble());
	if (fabs(x) >= FLT_MAX || fabs(y) >= FLT_MAX) {
		throw Exception("sprite pos err, filepath: %s, name: %s", val["filepath"].asString().c_str(), name.c_str());
	}

	// rotate
	float angle = static_cast<float>(val["angle"].asDouble());
	SetTransform(Vector(x, y), angle);

	// anchor
	m_is_anchor = val["anchor"].asBool();
}

void Sprite::Store(Json::Value& val) const
{
	val["name"] = name;
	val["tag"] = tag;
	val["clip"] = clip;

	rp->StoreToFile(val);

	val["position"]["x"] = m_pos.x;
	val["position"]["y"] = m_pos.y;

	val["angle"] = m_angle;

	val["x scale"] = m_scale.x;
	val["y scale"] = m_scale.y;

	val["x shear"] = m_shear.x;
	val["y shear"] = m_shear.y;

	val["x mirror"] = m_xMirror;
	val["y mirror"] = m_yMirror;

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

	if (!m_offset.IsValid()) {
		m_offset.Set(rect.CenterX(), rect.CenterY());
	}
	rect.Scale(m_scale.x, m_scale.y);
	rect.Shear(m_shear.x, m_shear.y);
	m_bounding->InitFromRect(rect);
	m_bounding->SetTransform(m_pos, m_offset, m_angle);
}

PropertySetting* Sprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::SetTransform(const Vector& position, float angle)
{
	if (m_pos != position) Translate(position - m_pos);
	if (m_angle != angle) Rotate(angle - m_angle);
}

void Sprite::SetScale(const Vector& scale)
{
	Vector dscale;
	dscale.x = scale.x / m_scale.x;
	dscale.y = scale.y / m_scale.y;

	Vector old_offset = m_offset;
	Vector new_offset(m_offset.x * dscale.x, m_offset.y * dscale.y);
	m_offset = new_offset;

	Translate(old_offset - new_offset);

	//////////////////////////////////////////////////////////////////////////

// 	sm::mat4 mat_old, mat_new;
// 	mat_old.scale(m_scale.x, m_scale.y);
// 	mat_new.scale(xScale, yScale);
// 
// 	Vector offset = Math2D::TransVector(m_offset, mat_new) - Math2D::TransVector(m_offset, mat_old);
// 
// 	m_offset += offset;
// 	translate(-offset);

	//////////////////////////////////////////////////////////////////////////

	m_scale = scale;
	BuildBounding();
 }

void Sprite::SetShear(const Vector& shear)
{
	sm::mat4 mat_old, mat_new;
	mat_old.Shear(m_shear.x, m_shear.y);
	mat_new.Shear(shear.x, shear.y);

	Vector offset = Math2D::TransVector(m_offset, mat_new) - Math2D::TransVector(m_offset, mat_old);

	m_offset += offset;
	Translate(-offset);

	m_shear = shear;
	BuildBounding();
}

void Sprite::SetOffset(const Vector& offset) 
{
	// rotate + offset -> offset + rotate	
	Vector old_center = GetCenter();
	m_offset = offset;
	Vector new_center = GetCenter();
	m_pos += (old_center - new_center);

	if (m_bounding) {
		m_bounding->SetTransform(m_pos, m_offset, m_angle);
	}
}

bool Sprite::IsContain(const Vector& pos) const
{
	return m_bounding ? m_bounding->IsContain(pos) : false;
}

bool Sprite::IsIntersect(const Rect& rect) const
{
	return m_bounding ? m_bounding->IsIntersect(rect) : false;
}

void Sprite::Translate(const Vector& offset)
{
#ifdef OPEN_SCREEN_CACHE
	bool find = SpatialPartition::Instance()->Remove(this);
	if (find) {
		SpriteRenderer::InvalidRect(this);
	}
#endif // OPEN_SCREEN_CACHE

	if (m_observer)
		m_observer->Translate(this, offset);

	m_pos += offset;
	if (m_bounding) {
		m_bounding->SetTransform(m_pos, m_offset, m_angle);
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
		m_bounding->SetTransform(m_pos, m_offset, m_angle);
	}

#ifdef OPEN_SCREEN_CACHE
	SpatialPartition::Instance()->Insert(this);
#endif // OPEN_SCREEN_CACHE
}

void Sprite::SetMirror(bool xMirror, bool yMirror) 
{ 
	bool x_dirty = xMirror != m_xMirror,
		 y_dirty = yMirror != m_yMirror;

	if (x_dirty) {
		m_offset.x = -m_offset.x;
	}
	if (y_dirty) {
		m_offset.y = -m_offset.y;
	}

	m_xMirror = xMirror; m_yMirror = yMirror; 
	if (m_bounding) {
		m_bounding->SetMirror(x_dirty, y_dirty);
		m_bounding->SetTransform(m_pos, m_offset, m_angle);
	}
}

Vector Sprite::GetCenter() const
{
	Vector center_offset = Math2D::RotateVector(-m_offset, m_angle) + m_offset;
	Vector center = m_pos + center_offset;
	return center;
}

Rect Sprite::GetRect() const
{
	std::vector<Vector> bound;
	GetBounding()->GetBoundPos(bound);
	Rect rect;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		rect.Combine(bound[i]);
	}
	return rect;
}

void Sprite::GetTransMatrix(sm::mat4& mt) const
{
	const float xScale = m_xMirror ? -m_scale.x : m_scale.x,
		yScale = m_yMirror ? -m_scale.y : m_scale.y;

	Vector center = GetCenter();
	mt.SetTransformation(center.x, center.y, m_angle, 
		xScale, yScale, 0, 0, m_shear.x, m_shear.y);
}

sm::mat4 Sprite::GetTransInvMatrix() const
{
	sm::mat4 mat;
	mat.RotateZ(-m_angle);
	mat.Shear(-m_shear.x, -m_shear.y);
	mat.Translate(-m_pos.x/m_scale.x, -m_pos.y/m_scale.y, 0);
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