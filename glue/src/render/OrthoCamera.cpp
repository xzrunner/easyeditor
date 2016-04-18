#include "OrthoCamera.h"

namespace glue
{

OrthoCamera::OrthoCamera()
	: m_position(0, 0)
	, m_scale(1)
{
}

void OrthoCamera::OnSize(int width, int height)
{

}

void OrthoCamera::Reset()
{
	m_position.x = m_position.y = 0;
	m_scale = 1;
}

vec2 OrthoCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	vec2 proj;
	const int vx = x, vy = height - y;
	proj.x = (vx - (width >> 1)) * m_scale + m_position.x;
	proj.y = (vy - (height >> 1)) * m_scale + m_position.y;
	return proj;
}

vec2 OrthoCamera::TransPosProjectToScreen(const vec2& proj, int width, int height) const
{
	vec2 scr;
	float vx = (proj.x - m_position.x) / m_scale + (width >> 1),
		  vy = (proj.y - m_position.y) / m_scale + (height >> 1);
	scr.x = vx;
	scr.y = height - vy;
	return scr;
}

void OrthoCamera::Translate(const vec2& offset)
{
	m_position += offset * m_scale;
}

void OrthoCamera::Scale(float scale, int x, int y, int width, int height)
{
	float new_scale = m_scale * scale;
	m_position.x = (x - (width >> 1)) * m_scale + m_position.x - (x - (width >> 1)) * new_scale;
	m_position.y = (y - (height >> 1)) * m_scale + m_position.y - (y - (height >> 1)) * new_scale;
	m_scale = new_scale;
}

}