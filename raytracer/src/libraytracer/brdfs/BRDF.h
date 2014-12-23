#ifndef _RT_BRDF_H_
#define _RT_BRDF_H_

#include "utilities/RGBColor.h"
#include "utilities/Constants.h"
#include "utilities/Object.h"

namespace rt
{

class ShadeRec;
class Vector3D;

class BRDF : public Object
{
public:
	virtual ~BRDF() {}
	
	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

}; // BRDF

}

#endif // _RT_BRDF_H_