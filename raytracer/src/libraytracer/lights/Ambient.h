#ifndef _RT_AMBIENT_H_
#define _RT_AMBIENT_H_

#include "Light.h"

#include "utilities/RGBColor.h"

namespace rt
{

class Ambient : public Light
{
public:
	Ambient();

	virtual RGBColor L(const ShadeRec& sr) const;

	virtual Vector3D GetDirection(const ShadeRec& sr) const;

	virtual bool InShadow(const Ray& ray, const ShadeRec& sr) const;

	void ScaleRadiance(const float b);

private:
	float m_ls;
	RGBColor m_color;

}; // Ambient

inline Ambient::Ambient()
	: m_ls(1)
	, m_color(1, 1, 1)
{
}

inline RGBColor Ambient::
L(const ShadeRec& sr) const
{
	return m_ls * m_color;
}

inline Vector3D Ambient::
GetDirection(const ShadeRec& sr) const
{
	return Vector3D(0, 0, 0);
}

inline bool Ambient::
InShadow(const Ray& ray, const ShadeRec& sr) const
{
	// not implented yet
	return true;
}

inline void Ambient::
ScaleRadiance(const float b)
{
	m_ls = b;
}

}

#endif // _RT_AMBIENT_H_