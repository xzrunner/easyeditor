#ifndef _RT_DIRECTIONAL_H_
#define _RT_DIRECTIONAL_H_

#include "Light.h"

namespace rt
{

class Directional : public Light
{
public:
	Directional();

	virtual RGBColor L(const ShadeRec& sr) const;

	virtual Vector3D GetDirection(const ShadeRec& sr) const;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const;

	void ScaleRadiance(const float b);

	void SetDirection(const Vector3D& dir);

private:
	float m_ls;
	RGBColor m_color;

	Vector3D m_dir;

}; // Directional

inline Directional::
Directional()
	: m_ls(1)
	, m_color(1, 1, 1)
	, m_dir(0, 1, 0)
{
}

inline RGBColor Directional::
L(const ShadeRec& sr) const
{
	return m_ls * m_color;
}

inline Vector3D Directional::
GetDirection(const ShadeRec& sr) const
{
	return m_dir;
}

inline bool Directional::
InShadow(const Ray& ray, const ShadeRec& sr) const
{
	// not implented yet
	return true;
}

inline void Directional::
ScaleRadiance(const float b)
{
	m_ls = b;
}

inline void Directional::
SetDirection(const Vector3D& dir)
{
	m_dir = dir;
	m_dir.Normalize();
}

}

#endif // _RT_DIRECTIONAL_H_