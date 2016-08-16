#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "OBB.h"
#include "RenderFilter.h"
#include "FilterFactory.h"

namespace s2
{

Sprite::Sprite()
	: m_sym(NULL)
	, m_position(0, 0)
	, m_angle(0)
	, m_scale(1, 1)
	, m_shear(1, 1)
	, m_visible(true)
{
	m_offset.MakeInvalid();

	m_bounding = new OBB();
	m_bounding_dirty = true;

	m_shader.filter = FilterFactory::Instance()->Create(FM_NULL);
}

Sprite::Sprite(const Sprite& spr)
	: m_sym(NULL)
	, m_position(spr.m_position)
	, m_angle(spr.m_angle)
	, m_scale(spr.m_scale)
	, m_shear(spr.m_shear)
	, m_offset(spr.m_offset)
	, m_color(spr.m_color)
	, m_shader(spr.m_shader)
	, m_camera(spr.m_camera)
	, m_visible(spr.m_visible)
{
	if (spr.m_sym) {
		spr.m_sym->AddReference();
		m_sym = spr.m_sym;
	}

	m_bounding = spr.m_bounding->Clone();
	m_bounding_dirty = true;

	FilterMode fm = FM_NULL;
	if (spr.Shader().filter) {
		fm = spr.Shader().filter->GetMode();
	}
	m_shader.filter = FilterFactory::Instance()->Create(fm);
}

Sprite::Sprite(Symbol* sym)
	: m_sym(NULL)
	, m_position(0, 0)
	, m_angle(0)
	, m_scale(1, 1)
	, m_shear(1, 1)
	, m_visible(true)
{
	m_offset.MakeInvalid();

	m_bounding = new OBB();
	SetSymbol(sym);

	m_shader.filter = FilterFactory::Instance()->Create(FM_NULL);
}

Sprite::~Sprite()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}

	delete m_bounding;

	delete m_shader.filter;
}

void Sprite::Translate(const sm::vec2& offset)
{
	m_position += offset;
}

void Sprite::Rotate(float delta)
{
	m_angle += delta;
}

void Sprite::SetSymbol(Symbol* sym)
{
	cu::RefCountObjAssign(m_sym, sym);
	UpdateBounding();
}

const BoundingBox* Sprite::GetBounding() const 
{ 
	if (m_bounding_dirty) {
		UpdateBounding();
	}
	return m_bounding; 
}

// todo: m_sym->GetBounding too slow, should be cached
void Sprite::UpdateBounding() const
{
	if (!m_sym) {
		return;
	}

	sm::rect rect = m_sym->GetBounding(this);

	if (!m_offset.IsValid()) {
		m_offset = rect.Center();
	}

	m_bounding_dirty = false;
	m_bounding->Build(rect, m_position, m_angle, m_scale, m_shear, m_offset);	
}

sm::vec2 Sprite::GetCenter() const
{
	sm::vec2 center_offset = sm::rotate_vector(-m_offset, m_angle) + m_offset;
	sm::vec2 center = m_position + center_offset;
	return center;
}

void Sprite::SetPosition(const sm::vec2& pos)
{
	m_position = pos;

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);

	// lazy
	m_bounding_dirty = true;
}

void Sprite::SetAngle(float angle)
{
	m_angle = angle;

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);

	// lazy
	m_bounding_dirty = true;
}

void Sprite::SetScale(const sm::vec2& scale)
{
	const sm::vec2& old_scale = m_scale;
	if (old_scale.x != 0 && old_scale.y != 0) {
		sm::vec2 dscale;
		dscale.x = scale.x / m_scale.x;
		dscale.y = scale.y / m_scale.y;

		sm::vec2 old_offset = m_offset;
		sm::vec2 new_offset(m_offset.x * dscale.x, m_offset.y * dscale.y);
		m_offset = new_offset;

		Translate(old_offset - new_offset);
	}

	m_scale = scale;

	// lazy
	m_bounding_dirty = true;
}

void Sprite::SetShear(const sm::vec2& shear)
{
	sm::mat4 mat_old, mat_new;
	mat_old.Shear(m_shear.x, m_shear.y);
	mat_new.Shear(shear.x, shear.y);

	sm::vec2 offset = mat_new * m_offset - mat_old * m_offset;
	m_offset += offset;
	Translate(-offset);	

	m_shear = shear;

	// immediately
	m_bounding->SetTransform(m_position, m_offset, m_angle);
	
// 	// lazy
// 	m_bounding_dirty = true; 
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = GetCenter();
	m_offset = offset;
	sm::vec2 new_center = GetCenter();
	Translate(old_center - new_center);

	// immediately
	m_bounding->SetTransform(m_position, m_offset, m_angle);

// 	// lazy
// 	m_bounding_dirty = true; 
}

sm::mat4 Sprite::GetTransMatrix() const
{
	sm::vec2 center = GetCenter();
	sm::mat4 mt;
	mt.SetTransformation(center.x, center.y, m_angle, 
		m_scale.x, m_scale.y, 0, 0, m_shear.x, m_shear.y);
	return mt;
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

}