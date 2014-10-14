#define NOMINMAX 
#include <gl/GLee.h>

#include "ModelParametric.h"
#include "ParametricSurface.h"

namespace z3d
{

ModelParametric::ModelParametric(const ISurface* surface)
{
	Mesh mesh;

	// Create the VBO for the vertices.
	std::vector<float> vertices;
	surface->GenerateVertices(vertices, z3d::VertexFlagsNormals);
	glGenBuffers(1, &mesh.vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(vertices[0]),
		&vertices[0],
		GL_STATIC_DRAW);

	// Create a new VBO for the indices if needed.
	int index_count = surface->GetTriangleIndexCount();
	std::vector<GLushort> indices(index_count);
	surface->GenerateTriangleIndices(indices);
	glGenBuffers(1, &mesh.index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		index_count * sizeof(GLushort),
		&indices[0],
		GL_STATIC_DRAW);

	mesh.index_count = surface->GetTriangleIndexCount();

	mesh.vertices = vertices;
	mesh.indices = indices;

	for (int i = 0; i < mesh.vertices.size(); ++i) {
		mesh.vertices[i] *= 0.0001f;
	}

	m_meshes.push_back(mesh);
}

// ModelParametric::ModelParametric(const ISurface* surface)
// {
// 	Mesh mesh;
// 
// 	float vertices[] = {
// 		0, 0, -0.1f, 0, 0, 1,
// 		100, 0, 0.5f,  0, 0, 1,
// 		0, 100, -0.9f, 0, 0, 1,
// 	};
// 	glGenBuffers(1, &mesh.vertex_buffer);
// 	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer);
// 	glBufferData(GL_ARRAY_BUFFER,
// 				18 * sizeof(float),
// 				&vertices[0],
// 				GL_STATIC_DRAW);
// 
// 	GLushort indices[] = {
// 		0, 1, 2
// 	};
// 	glGenBuffers(1, &mesh.index_buffer);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);
// 	glBufferData(
// 		GL_ELEMENT_ARRAY_BUFFER,
// 		3 * sizeof(GLushort),
// 		&indices[0],
// 		GL_STATIC_DRAW);
// 
// 	mesh.index_count = 1;
// 
// 	m_meshes.push_back(mesh);
// }

}