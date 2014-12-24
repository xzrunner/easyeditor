#ifndef _RT_MESH_H_
#define _RT_MESH_H_

#include "maths/Point3D.h"
#include "maths/Normal.h"

namespace rt
{

class Mesh
{
public:
	vector<Point3D> 		vertices;				// mesh vertices 
	vector<Normal> 			normals;				// average normal at each vertex;
	vector<float>			u;						// u texture coordinate at each vertex
	vector<float>			v;						// v texture coordinate at each vertex
	vector<vector<int> > 	vertex_faces;			// the triangles shared by each vertex
	int 					num_vertices; 			// number of vertices
	int 					num_triangles; 			// number of triangles

}; // Mesh

}

#endif // _RT_MESH_H_