#include "Cube.h"

namespace e3d
{

void Cube::GetSize(vec3& min, vec3& max) const
{
	min.x = m_xmin;
	min.y = m_ymin;
	min.z = m_zmin;

	max.x = m_xmax;
	max.y = m_ymax;
	max.z = m_zmax;
}

}