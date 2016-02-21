#ifndef _EASY3D_MATERIAL_H_
#define _EASY3D_MATERIAL_H_

#include <ee/Vector3D.h>
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

	ee::vec3 ambient;
	ee::vec3 diffuse;
	ee::vec3 specular;

	ee::TextureImgData m_diffuse_tex;
};

}

#endif // _EASY3D_MATERIAL_H_