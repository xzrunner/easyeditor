#ifndef _RT_REFLECTIVE_H_
#define _RT_REFLECTIVE_H_

#include "Material.h"

namespace rt
{

class PerfectSpecular;

class Reflective : public Material
{
public:
	
private:
	PerfectSpecular* reflective_brdf;

}; // Reflective

}

#endif // _RT_REFLECTIVE_H_