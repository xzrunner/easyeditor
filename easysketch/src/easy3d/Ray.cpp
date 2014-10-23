#include "Ray.h"

namespace e3d
{

void Ray::ChangeCoordSystem(const Quaternion& rot)
{
	mat4 rot_mat = (-rot).ToMatrix();
	m_start = rot_mat * m_start;
	m_dir = rot_mat * m_dir;
}

}