#ifndef _RT_PHONG_H_
#define _RT_PHONG_H_

#include "Material.h"

namespace rt
{

class Lambertian;
class GlossySpecular;

class Phong : public Material
{
public:

private:
	Lambertian*			ambient_brdf;
	Lambertian*			diffuse_brdf;
	GlossySpecular*		specular_brdf;

}; // Phong

}

#endif // _RT_PHONG_H_