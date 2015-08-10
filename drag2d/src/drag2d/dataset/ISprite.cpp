#include "ISprite.h"
#include "ISymbol.h"

#include "common/Math.h"
#include "common/Matrix.h"
#include "common/color_trans.h"
#include "common/Exception.h"
#include "common/config.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"
#include "dataset/SpriteFactory.h"
#include "view/SpritePropertySetting.h"

#include <cmath>

namespace d2d
{

ISprite::ISprite()
	: m_observer(NULL)
{
	multiCol.set(1, 1, 1, 1);
	addCol.set(0, 0, 0, 0);
	r_trans.set(1, 0, 0, 1);
	g_trans.set(0, 1, 0, 1);
	b_trans.set(0, 0, 1, 1);

	clip = false;

	visiable = editable = true;

	m_angle = 0.0f;
	m_scale.set(1, 1);
	m_shear.set(0, 0);
	m_xMirror = m_yMirror = false;
	m_perspective.set(0, 0);
	m_bounding = NULL;

	m_blend_mode = BM_NORMAL;
	m_filter_mode = FM_NORMAL;
}

ISprite::ISprite(const ISprite& sprite)
	: m_observer(NULL)
{
	name = sprite.name;
	multiCol = sprite.multiCol;
	addCol = sprite.addCol;
	clip = sprite.clip;
	r_trans = sprite.r_trans;
	g_trans = sprite.g_trans;
	b_trans = sprite.b_trans;

	visiable = sprite.visiable;
	editable = sprite.editable;
	tag = sprite.tag;

	m_pos = sprite.m_pos;
	m_angle = sprite.m_angle;
	m_offset = sprite.m_offset;
	m_scale = sprite.m_scale;
	m_shear = sprite.m_shear;
	m_xMirror = sprite.m_xMirror;
	m_yMirror = sprite.m_yMirror;
	m_perspective = sprite.m_perspective;
	m_bounding = sprite.m_bounding->Clone();

	m_blend_mode = sprite.m_blend_mode;
	m_filter_mode = sprite.m_filter_mode;
}

ISprite::~ISprite()
{
	delete m_bounding;
	SpriteFactory::Instance()->remove(this);
}

ISprite* ISprite::Clone() const
{
	return NULL;
}

void ISprite::ClearUserData(bool deletePtr)
{
	delete m_userData, m_userData = NULL;
}

void ISprite::Load(const Json::Value& val)
{
	// info
	name = val["name"].asString();
	tag = val["tag"].asString();
	clip = val["clip"].asBool();

	// color
	std::string str = val["multi color"].asString();
	if (str.empty())
		multiCol = Colorf(1, 1, 1, 1);
	else
		multiCol = transColor(str, PT_BGRA);
	str = val["add color"].asString();
	if (str.empty())
		addCol = Colorf(0, 0, 0, 0);
	else
		addCol = transColor(str, PT_ABGR);

	str = val["r trans"].asString();
	if (str.empty())
		r_trans = Colorf(1, 0, 0, 1);
	else
		r_trans = transColor(str, PT_RGBA);
	str = val["g trans"].asString();
	if (str.empty())
		g_trans = Colorf(0, 1, 0, 1);
	else
		g_trans = transColor(str, PT_RGBA);
	str = val["b trans"].asString();
	if (str.empty())
		b_trans = Colorf(0, 0, 1, 1);
	else
		b_trans = transColor(str, PT_RGBA);

	// scale
	float sx, sy;
	if (val["scale"].isNull())
	{
		sx = val["x scale"].asDouble();
		sy = val["y scale"].asDouble();
	}
	else
	{
		sx = sy = val["scale"].asDouble();
	}
	SetScale(sx, sy);

	// shear
	float kx, ky;
	if (!val["x shear"].isNull())
	{
		kx = val["x shear"].asDouble();
		ky = val["y shear"].asDouble();
	}
	else
	{
		kx = ky = 0;
	}
	SetShear(kx, ky);

	// mirror
	bool mx = val["x mirror"].asBool();
	bool my = val["y mirror"].asBool();
	SetMirror(mx, my);

	// perspective
	if (!val["x perspective"].isNull())
	{
		Vector persp;
		persp.x = val["x perspective"].asDouble();
		persp.y = val["y perspective"].asDouble();
		SetPerspective(persp);
	}

	// offset
	float ox, oy;
	if (!val["x offset"].isNull())
	{
		ox = val["x offset"].asDouble();
		oy = val["y offset"].asDouble();
	}
	else
	{
		ox = oy = 0;
	}
	SetOffset(Vector(ox, oy));

	// translate
	float x = val["position"]["x"].asDouble();
	float y = val["position"]["y"].asDouble();
	if (fabs(x) >= FLT_MAX || fabs(y) >= FLT_MAX) {
		throw Exception("sprite pos err, filepath: %s, name: %s", val["filepath"].asString().c_str(), name.c_str());
	}

	// rotate
	float angle = val["angle"].asDouble();
	SetTransform(Vector(x, y), angle);

	// filter
	if (!val["filter"].isNull()) {
		std::string disc = val["filter"].asString();
		if (Config::Instance()->IsRenderOpen()) {
			m_filter_mode = FilterModes::Instance()->GetIDFromNameEN(disc);
		}
	}
}

void ISprite::Store(Json::Value& val) const
{
	val["name"] = name;
	val["tag"] = tag;
	val["clip"] = clip;

	val["multi color"] = transColor(multiCol, PT_BGRA);
	val["add color"] = transColor(addCol, PT_ABGR);
	val["r trans"] = transColor(r_trans, PT_RGBA);
	val["g trans"] = transColor(g_trans, PT_RGBA);
	val["b trans"] = transColor(b_trans, PT_RGBA);

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

	if (Config::Instance()->IsRenderOpen()) {
		val["filter"] = FilterModes::Instance()->GetNameENFromID(m_filter_mode);
	}
}

void ISprite::BuildBounding()
{
	if (!m_bounding) {
		m_bounding = BVFactory::createBV(e_obb);
	}
	const ISymbol& symbol = GetSymbol();
	Rect rect(symbol.GetSize(this));
	if (!rect.isValid()) {
		return;
	}

	if (m_offset.x == 0 && m_offset.y == 0) {
		m_offset.set(rect.xCenter(), rect.yCenter());
	}
	rect.scale(m_scale.x, m_scale.y);
	rect.shear(m_shear.x, m_shear.y);
	m_bounding->initFromRect(rect);
	m_bounding->SetTransform(m_pos, m_offset, m_angle);
}

IPropertySetting* ISprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void ISprite::SetTransform(const Vector& position, float angle)
{
	if (m_pos != position) Translate(position - m_pos);
	if (m_angle != angle) Rotate(angle - m_angle);
}

void ISprite::SetScale(float xScale, float yScale)
{
	Vector dscale;
	dscale.x = xScale / m_scale.x;
	dscale.y = yScale / m_scale.y;

	Vector old_offset = m_offset;
	Vector new_offset(m_offset.x * dscale.x, m_offset.y * dscale.y);
	m_offset = new_offset;

	Translate(old_offset - new_offset);

	//////////////////////////////////////////////////////////////////////////

// 	Matrix mat_old, mat_new;
// 	mat_old.scale(m_scale.x, m_scale.y);
// 	mat_new.scale(xScale, yScale);
// 
// 	Vector offset = Math::transVector(m_offset, mat_new) - Math::transVector(m_offset, mat_old);
// 
// 	m_offset += offset;
// 	translate(-offset);

	//////////////////////////////////////////////////////////////////////////

	m_scale.set(xScale, yScale);
	BuildBounding();
 }

void ISprite::SetShear(float xShear, float yShear)
{
	Matrix mat_old, mat_new;
	mat_old.shear(m_shear.x, m_shear.y);
	mat_new.shear(xShear, yShear);

	Vector offset = Math::transVector(m_offset, mat_new) - Math::transVector(m_offset, mat_old);

	m_offset += offset;
	Translate(-offset);

	m_shear.set(xShear, yShear);
	BuildBounding();
}

bool ISprite::IsContain(const Vector& pos) const
{
	return m_bounding ? m_bounding->isContain(pos) : false;
}

bool ISprite::IsIntersect(const Rect& rect) const
{
	return m_bounding ? m_bounding->isIntersect(rect) : false;
}

void ISprite::Translate(const Vector& offset)
{
	if (m_observer)
		m_observer->Translate(this, offset);

	m_pos += offset;
	if (m_bounding) {
		m_bounding->SetTransform(m_pos, m_offset, m_angle);
	}
}

void ISprite::Rotate(float delta)
{
	if (m_observer)
		m_observer->Rotate(this, delta);

	m_angle += delta;

	if (m_bounding) {
		m_bounding->SetTransform(m_pos, m_offset, m_angle);
	}
}

void ISprite::SetOffset(const Vector& offset) 
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

void ISprite::SetMirror(bool xMirror, bool yMirror) 
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

Vector ISprite::GetCenter() const
{
	d2d::Vector center_offset = Math::rotateVector(-m_offset, m_angle) + m_offset;
	d2d::Vector center = m_pos + center_offset;
	return center;
}

d2d::Rect ISprite::GetRect() const
{
	std::vector<Vector> bound;
	GetBounding()->getBoundPos(bound);
	d2d::Rect rect;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		rect.combine(bound[i]);
	}
	return rect;
}

void ISprite::GetTransMatrix(Matrix& mt) const
{
	const float xScale = m_xMirror ? -m_scale.x : m_scale.x,
		yScale = m_yMirror ? -m_scale.y : m_scale.y;

	d2d::Vector center = GetCenter();
	mt.setTransformation(center.x, center.y, m_angle, 
		xScale, yScale, 0, 0, m_shear.x, m_shear.y);
}

Matrix ISprite::GetTransInvMatrix() const
{
	d2d::Matrix mat;
	mat.rotate(-m_angle);
	mat.shear(-m_shear.x, -m_shear.y);
	mat.translate(-m_pos.x/m_scale.x, -m_pos.y/m_scale.y);
	mat.scale(1/m_scale.x, 1/m_scale.y);
	return mat;
}

template<typename T>
void ISprite::SetSymbol(T** dst, d2d::ISymbol* src)
{
	if (src == dst) {
		return;
	}
	T* sym = dynamic_cast<T*>(src);
	if (!sym) {
		return;
	}
	d2d::obj_assign<T>(*dst, sym);
	BuildBounding();
}

//////////////////////////////////////////////////////////////////////////
// class SpriteCmp
//////////////////////////////////////////////////////////////////////////

SpriteCmp::SpriteCmp(Type type /*= e_file*/)
	: m_type(type)
{
}

bool SpriteCmp::operator() (const ISprite* s0, const ISprite* s1) const
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

} // d2d