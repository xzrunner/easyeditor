#ifndef _E3D_AABB_H_
#define _E3D_AABB_H_

#include "Vector.h"
#include "Matrix.h"

#include <float.h>

namespace e3d
{

class AABB
{
public:
	AABB();

	void MakeInfinite();

	void Combine(const vec3& pos);

	void Translate(const vec3& offset);
	void Rotate(const mat4& mat);

	const vec3 Max() const;
	const vec3 Min() const;

private:
	vec3 m_min, m_max;

}; // AABB

inline AABB::AABB() 
{ 
	MakeInfinite(); 
}

inline void AABB::MakeInfinite() 
{
	for (int i = 0; i < 3; ++i) {
		m_min[i] = FLT_MAX;
		m_max[i] = -FLT_MAX;
	}
}

inline void AABB::Combine(const vec3& pos) 
{
	for (int i = 0; i < 3; ++i) {
		if (pos[i] < m_min[i]) {
			m_min[i] = pos[i];
		}
		if (pos[i] > m_max[i]) {
			m_max[i] = pos[i];
		}
	}
}

inline void AABB::Translate(const vec3& offset)
{
	m_min += offset;
	m_max += offset;
}

inline void AABB::Rotate(const mat4& mat)
{
	m_min = mat * m_min;
	m_max = mat * m_max;
}

inline const vec3 AABB::Max() const 
{ 
	return m_max; 
}

inline const vec3 AABB::Min() const 
{ 
	return m_min; 
}

}

#endif // _E3D_AABB_H_