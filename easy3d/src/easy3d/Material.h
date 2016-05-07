#ifndef _EASY3D_MATERIAL_H_
#define _EASY3D_MATERIAL_H_

#include <ee/TextureImgData.h>

namespace e3d 
{

struct Material
{
	Material()
		: ambient(0.2f, 0, 0)
		, diffuse(0.3f, 0, 0)
		, specular(0.4f, 0, 0)
	{}

	sm::vec3 ambient;
	sm::vec3 diffuse;
	sm::vec3 specular;

	ee::TextureImgData m_diffuse_tex;
};

}

#endif // _EASY3D_MATERIAL_H_