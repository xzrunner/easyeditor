#ifndef _EASY3D_MESH_H_
#define _EASY3D_MESH_H_

#include "Material.h"

namespace e3d 
{

struct Mesh
{
	GLuint vertex_buffer;
	GLuint index_buffer;
	int index_count;

	Material material;
};

}

#endif // _EASY3D_MESH_H_