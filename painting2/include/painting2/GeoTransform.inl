#pragma once

#include <SM_Calc.h>

namespace pt2
{

inline
void GeoTransform::SetPosition(const sm::vec2& pos) 
{
	if (pos != m_srt.position) {
		m_srt.position = pos;
		UpdateCenter();
		UpdateMatrix();
	}
}

inline
void GeoTransform::SetAngle(float angle) 
{
	if (angle != m_srt.angle) {
		m_srt.angle = angle;
		UpdateCenter();
		UpdateMatrix();
	}
}

inline
void GeoTransform::SetScale(const sm::vec2& scale) 
{ 
	if (scale != m_srt.scale) {
		m_srt.scale = scale;
		UpdateMatrix();
	}
}

inline
void GeoTransform::SetShear(const sm::vec2& shear) 
{
	if (shear != m_srt.shear) {
		m_srt.shear = shear;
		UpdateMatrix();
	}
}

inline
void GeoTransform::SetOffset(const sm::vec2& offset) 
{ 
	if (offset != m_srt.offset) {
		m_srt.offset = offset;
		UpdateCenter();
		UpdateMatrix();
	}
}

inline
void GeoTransform::SetSRT(const SRT& srt)
{
	m_srt = srt;
	UpdateMatrix();
}

inline
void GeoTransform::UpdateCenter()
{
	m_srt.center = m_srt.position + sm::rotate_vector(-m_srt.offset, m_srt.angle) + m_srt.offset;
}

inline
void GeoTransform::UpdateMatrix()
{
	m_mat.SetTransformation(m_srt.center.x, m_srt.center.y, m_srt.angle, m_srt.scale.x, 
		m_srt.scale.y, 0, 0, m_srt.shear.x, m_srt.shear.y);
}

}