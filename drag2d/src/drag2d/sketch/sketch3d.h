#ifndef _DRAG2D_SKETCH3D_H_
#define _DRAG2D_SKETCH3D_H_

#include "Vector.h"

#include <vector>

typedef unsigned int uint32;

namespace z3d 
{

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Mesh
{
	uint32 vertex_buffer;
	uint32 index_buffer;
	uint32 index_count;

	Material material;
};

class IModel
{
public:
	virtual const std::vector<Mesh>& GetAllMeshes() const = 0;
	virtual ~IModel() {}
};

}

#endif // _DRAG2D_SKETCH3D_H_