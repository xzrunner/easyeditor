#ifndef _LIBSKETCH_CUBE_H_
#define _LIBSKETCH_CUBE_H_

#include <drag2d.h>

namespace z3d
{

class Cube
{
public:
	Cube() { MakeInfinite(); }

	void MakeInfinite() {
		m_xmin = m_ymin = m_zmin = FLT_MAX;
		m_xmax = m_ymax = m_zmax = - FLT_MAX;
	}

	void Combine(const vec3& pos) {
		if (pos.x < m_xmin) m_xmin = pos.x;
		if (pos.x > m_xmax) m_xmax = pos.x;
		if (pos.y < m_ymin) m_ymin = pos.y;
		if (pos.y > m_ymax) m_ymax = pos.y;
		if (pos.z < m_zmin) m_zmin = pos.z;
		if (pos.z > m_zmax) m_zmax = pos.z;
	}

private:
	float m_xmin, m_xmax;
	float m_ymin, m_ymax;
	float m_zmin, m_zmax;

}; // Cube

}

#endif // _LIBSKETCH_CUBE_H_