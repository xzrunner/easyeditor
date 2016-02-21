#include "Ray.h"

namespace e3d
{

void Ray::ChangeCoordSystem(const ee::Quaternion& rot)
{
	ee::mat4 rot_mat = (-rot).ToMatrix();

	ee::vec3 old_start = rot_mat * m_start;
	ee::vec3 new_start = rot_mat.Invert() * m_start;
	m_start = old_start;
	m_dir = (-rot).Rotated(m_dir);
}

ee::vec3 Ray::Dir() const
{
 	ee::mat4 mat = (m_dir).ToMatrix();
 	ee::vec3 dir = mat * m_dir_vec;
 	return dir;
}

}