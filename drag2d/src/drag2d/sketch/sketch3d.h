#ifndef _DRAG2D_SKETCH3D_H_
#define _DRAG2D_SKETCH3D_H_

#include "Vector.h"
#include "common/Object.h"

#include <vector>

typedef unsigned int uint32;

namespace z3d 
{

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	Material()
		: ambient(0.2, 0, 0)
		, diffuse(0.3, 0, 0)
		, specular(0.4, 0, 0)
	{}
};

struct Mesh
{
	uint32 vertex_buffer;
	uint32 index_buffer;
	uint32 index_count;

	Material material;
};

class IModel : public d2d::Object
{
public:
	virtual const std::vector<Mesh>& GetAllMeshes() const = 0;
	virtual ~IModel() {}
};

}

#endif // _DRAG2D_SKETCH3D_H_