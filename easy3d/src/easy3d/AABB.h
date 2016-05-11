#ifndef _EASY3D_AABB_H_
#define _EASY3D_AABB_H_

#include <SM_Matrix.h>

#include <float.h>

namespace e3d
{

class AABB
{
public:
	AABB();

	void MakeInfinite();

	void Combine(const sm::vec3& pos);

	void Translate(const sm::vec3& offset);
	void Rotate(const sm::mat4& mat);

	const sm::vec3 Max() const;
	const sm::vec3 Min() const;

private:
	sm::vec3 m_min, m_max;

}; // AABB

inline AABB::AABB() 
{ 
	MakeInfinite(); 
}

inline void AABB::MakeInfinite() 
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

inline const sm::vec3 AABB::Max() const 
{ 
	return m_max; 
}

inline const sm::vec3 AABB::Min() const 
{ 
	return m_min; 
}

}

#endif // _EASY3D_AABB_H_