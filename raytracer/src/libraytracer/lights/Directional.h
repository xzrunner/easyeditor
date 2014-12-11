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

	virtual float G(const ShadeRec& sr) const;
	virtual float Pdf(const ShadeRec& sr) const;

	void ScaleRadiance(const float b);

	void SetDirection(const Vector3D& dir);

private:
	float m_ls;
	RGBColor m_color;

	Vector3D m_dir;

}; // Directional

}

#endif // _RT_DIRECTIONAL_H_