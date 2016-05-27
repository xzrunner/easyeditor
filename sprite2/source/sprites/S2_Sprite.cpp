#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "OBB.h"

namespace s2
{

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
	, m_ud(spr.m_ud)
{
	m_bounding = new OBB(*static_cast<OBB*>(spr.m_bounding));
	m_bounding_dirty = false;
}

Sprite::Sprite(void* ud)
	: m_sym(NULL)
	, m_position(0, 0)
	, m_angle(0)
	, m_scale(1, 1)
	, m_shear(0, 0)
	, m_ud(ud)
{
	m_bounding = new OBB();
	m_bounding_dirty = true;
}

Sprite::~Sprite()
{
	delete m_bounding;
}

void Sprite::SetPosition(const sm::vec2& pos)
{
	m_position = pos;
	m_bounding_dirty = true;
}

void Sprite::SetAngle(float angle)
{
	m_angle = angle;
	m_bounding_dirty = true;
}

void Sprite::SetScale(const sm::vec2& scale)
{
	m_scale = scale;
	m_bounding_dirty = true;
}

void Sprite::SetShear(const sm::vec2& shear)
{
	m_shear = shear;
	m_bounding_dirty = true;
}

// void Sprite::MultiplyRenderParams(const RenderParams& src, RenderParams& dst) const
// {
// 	
// }

void Sprite::UpdateBounding()
{
	if (m_sym) {
		m_bounding->Build(m_sym->GetSize(this), m_position, m_angle, m_scale, m_shear, m_offset);	
	}
}

}