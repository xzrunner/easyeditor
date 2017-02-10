#ifndef _POLYMESH_TRIANGLES_H_
#define _POLYMESH_TRIANGLES_H_

#include "MeshData.h"
#include "Vertex.h"

#include <stdint.h>

namespace pm
{

class Triangles : public MeshData
{
public:
	Triangles();

	virtual void Dump(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const;

	virtual void LoadFromTransform(const MeshTransform& transform);
	virtual void StoreToTransform(MeshTransform& transform) const;

	static Triangles* Create(const std::vector<sm::vec2>& vertices, 
		                     const std::vector<sm::vec2>& texcoords, 
							 const std::vector<int>& triangles);

private:
	uint16_t  m_vert_num;
	uint16_t  m_tri_num;
	uint16_t* m_triangles;
	Vertex    m_vertices[1];

	friend class TrianglesMesh;

}; // Triangles

}

#endif // _POLYMESH_TRIANGLES_H_