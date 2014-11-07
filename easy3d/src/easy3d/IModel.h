#ifndef _E2D_IMODEL_H_
#define _E2D_IMODEL_H_

#include "Vector.h"

#include <drag2d.h>

namespace e3d 
{
 
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	Material()
		: ambient(0.2f, 0, 0)
		, diffuse(0.3f, 0, 0)
		, specular(0.4f, 0, 0)
	{}
};

struct Mesh
{
	GLuint vertex_buffer;
	GLuint index_buffer;
	int index_count;

	Material material;
};

class IModel : public d2d::Object
{
public:
	virtual const std::vector<Mesh>& GetAllMeshes() const = 0;
	virtual ~IModel() {}
};

}

#endif // _E2D_IMODEL_H_