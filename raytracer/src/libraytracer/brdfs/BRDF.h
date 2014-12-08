#ifndef _RT_BRDF_H_
#define _RT_BRDF_H_

#include "utilities/RGBColor.h"
#include "utilities/Constants.h"

namespace rt
{

class ShadeRec;
class Vector3D;

class BRDF
{
public:
	
	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const = 0;

}; // BRDF

inline RGBColor BRDF::
f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return BLACK;
}

}

#endif // _RT_BRDF_H_