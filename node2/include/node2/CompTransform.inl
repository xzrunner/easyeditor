#pragma once

#include <SM_Calc.h>

namespace n2
{

inline
void CompTransform::UpdateCenter()
{
	m_srt.center = m_srt.position + sm::rotate_vector(-m_srt.offset, m_srt.angle) + m_srt.offset;
}

inline
void CompTransform::UpdateMatrix()
{
	m_mat.SetTransformation(m_srt.center.x, m_srt.center.y, m_srt.angle, m_srt.scale.x, 
		m_srt.scale.y, 0, 0, m_srt.shear.x, m_srt.shear.y);
}


/************************************************************************/
/* class CompTransform::SRT                                             */
/************************************************************************/

inline
CompTransform::SRT::SRT()
{
	Reset();
}

inline
void CompTransform::SRT::Reset()
{
	position.Set(0, 0);
	angle = 0;
	scale.Set(1, 1);
	shear.Set(0, 0);
	offset.Set(0, 0);
	center.Set(0, 0);
}

}