#include <gl/glew.h>

#include "ModelParametric.h"
#include "ParametricSurface.h"
#include "AABB.h"

namespace e3d
{

ModelParametric::ModelParametric()
{

}

ModelParametric::ModelParametric(const ISurface* surface, AABB& aabb)
{
	Mesh mesh;

	unsigned char flag = e3d::VertexFlagsNormals;

	// Create the VBO for the vertices.
	std::vector<float> vertices;
	surface->GenerateVertices(vertices, flag);

	GLuint buf_id;
	glGenBuffers(1, &buf_id);
	mesh.vertex_buffer = buf_id;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(vertices[0]),
		&vertices[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a new VBO for the indices if needed.
	int index_count = surface->GetTriangleIndexCount();
	std::vector<GLushort> indices(index_count);
	surface->GenerateTriangleIndices(indices);
	glGenBuffers(1, &buf_id);
	mesh.index_buffer = buf_id;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		index_count * sizeof(GLushort),
		&indices[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mesh.index_count = surface->GetTriangleIndexCount();

	m_meshes.push_back(mesh);

	// Init aabb
	int step = 3;
	if (flag == e3d::VertexFlagsNormals) {
		step = 6;
	} else if (flag == e3d::VertexFlagsTexCoords) {
		step = 8;
	}
	for (int i = 0, n = vertices.size(); i < n; )
	{
		ee::vec3 pos;
		pos.x = vertices[i];
		pos.y = vertices[i+1];
		pos.z = vertices[i+2];
		aabb.Combine(pos);
		i += step;
	}
}

}