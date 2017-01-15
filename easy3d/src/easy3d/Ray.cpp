#include "Ray.h"

#include <SM_Matrix.h>

namespace e3d
{

void Ray::ChangeCoordSystem(const sm::Quaternion& rot)
{
	sm::mat4 rot_mat(-rot);

	sm::vec3 old_start = rot_mat * m_start;
	sm::vec3 new_start = rot_mat.Inverted() * m_start;
	m_start = old_start;
	m_dir = (-rot).Rotated(m_dir);
}

sm::vec3 Ray::Dir() const
{
 	sm::mat4 mat(m_dir);
 	sm::vec3 dir = mat * m_dir_vec;
 	return dir;
}

}