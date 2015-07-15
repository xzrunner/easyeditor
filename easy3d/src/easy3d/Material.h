#ifndef _EASY3D_MATERIAL_H_
#define _EASY3D_MATERIAL_H_

#include "Vector.h"

#include <drag2d.h>

namespace e3d 
{

struct Material
{
	Material()
		: ambient(0.2f, 0, 0)
		, diffuse(0.3f, 0, 0)
		, specular(0.4f, 0, 0)
	{}

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	d2d::TextureImgData m_diffuse_tex;
};

}

#endif // _EASY3D_MATERIAL_H_