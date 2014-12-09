#include "Checker3D.h"

#include "utilities/ShadeRec.h"

namespace rt
{

RGBColor Checker3D::GetColor(const ShadeRec& sr) const
{
	float eps = -0.000187453738f;
	float x = sr.local_hit_point.x + eps;
	float y = sr.local_hit_point.y + eps;
	float z = sr.local_hit_point.z + eps;

	if (((int) floor(x / m_size) + (int) floor(y / m_size) + (int) floor(z / m_size)) % 2 == 0) {
		return m_color1;
	} else {
		return m_color2;
	}
}

}