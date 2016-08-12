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
	, m_shear(0, 0)
	, m_offset(0, 0)
	, m_visible(true)
{
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

	m_bounding = new OBB(*static_cast<OBB*>(spr.m_bounding));
	m_bounding_dirty = false;

	FilterMode fm = FM_NULL;
	if (spr.Shader().filter) {
		fm = spr.Shader().filter->GetMode();
	}
	m_shader.filter = FilterFactory::Instance()->Create(fm);
}

Sprite::~Sprite()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}

	delete m_bounding;

	delete m_shader.filter;
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

// todo: m_sym->GetBounding too slow, should be cached
void Sprite::UpdateBounding()
{
	if (m_sym) {
		m_bounding->Build(m_sym->GetBounding(this), m_position, m_angle, m_scale, m_shear, m_offset);	
	}
}

}