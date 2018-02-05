#pragma once

namespace pt3
{

inline AABB::AABB() 
{ 
	MakeEmpty(); 
}

inline AABB::AABB(const sm::vec3& min, const sm::vec3& max)
	: m_min(min)
	, m_max(max)
{
}

inline void AABB::MakeEmpty() 
{
	for (int i = 0; i < 3; ++i) {
		m_min.xyz[i] = FLT_MAX;
		m_max.xyz[i] = -FLT_MAX;
	}
}

inline void AABB::Combine(const sm::vec3& pos) 
{
	for (int i = 0; i < 3; ++i) {
		if (pos.xyz[i] < m_min.xyz[i]) {
			m_min.xyz[i] = pos.xyz[i];
		}
		if (pos.xyz[i] > m_max.xyz[i]) {
			m_max.xyz[i] = pos.xyz[i];
		}
	}
}

inline void AABB::Translate(const sm::vec3& offset)
{
	m_min += offset;
	m_max += offset;
}

inline void AABB::Rotate(const sm::mat4& mat)
{
	m_min = mat * m_min;
	m_max = mat * m_max;
}

inline const sm::vec3& AABB::Max() const 
{ 
	return m_max; 
}

inline const sm::vec3& AABB::Min() const 
{ 
	return m_min; 
}

}