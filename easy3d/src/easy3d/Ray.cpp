#include "Ray.h"

namespace e3d
{

void Ray::ChangeCoordSystem(const Quaternion& rot)
{
	mat4 rot_mat = (-rot).ToMatrix();

	vec3 old_start = rot_mat * m_start;
	vec3 new_start = rot_mat.Invert() * m_start;
	m_start = old_start;
	m_dir = (-rot).Rotated(m_dir);
}

vec3 Ray::Dir() const
{
 	mat4 mat = (m_dir).ToMatrix();
 	vec3 dir = mat * m_dir_vec;
 	return dir;
}

}