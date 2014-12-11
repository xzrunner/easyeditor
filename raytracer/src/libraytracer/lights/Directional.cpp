#include "Directional.h"

namespace rt
{

Directional::Directional()
	: m_ls(1)
	, m_color(1, 1, 1)
	, m_dir(0, 1, 0)
{
}

RGBColor Directional::L(const ShadeRec& sr) const
{
	return m_ls * m_color;
}

Vector3D Directional::GetDirection(const ShadeRec& sr) const
{
	return m_dir;
}

bool Directional::InShadow(const Ray& ray, const ShadeRec& sr) const
{
	// not implented yet
	return true;
}

float Directional::G(const ShadeRec& sr) const
{
	return 5.5f;
}

float Directional::Pdf(const ShadeRec& sr) const
{
	return 5.5f;
}

void Directional::ScaleRadiance(const float b)
{
	m_ls = b;
}

void Directional::SetDirection(const Vector3D& dir)
{
	m_dir = dir;
	m_dir.Normalize();
}

}