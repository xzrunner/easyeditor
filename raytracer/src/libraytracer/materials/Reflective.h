#ifndef _RT_REFLECTIVE_H_
#define _RT_REFLECTIVE_H_

#include "Phong.h"

namespace rt
{

class PerfectSpecular;

class Reflective : public Phong
{
public:
	Reflective();
	virtual ~Reflective();
	
	virtual RGBColor Shade(const ShadeRec& sr) const;
	virtual RGBColor AreaLightShade(const ShadeRec& sr) const;

	void SetKr(const float k);
	void SetCr(const RGBColor& c);

private:
	PerfectSpecular* m_reflective_brdf;

}; // Reflective

}

#endif // _RT_REFLECTIVE_H_