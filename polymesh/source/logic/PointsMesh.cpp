#include "PointsMesh.h"

#include <SM_Triangulation.h>

namespace pm
{

PointsMesh::PointsMesh(const std::vector<sm::vec2>& outline, const std::vector<sm::vec2>& points, float width, float height)
{
	std::vector<sm::vec2> vertices;
	std::vector<int> triangles;
	sm::triangulate_points(outline, points, vertices, triangles);

	std::vector<sm::vec2> texcoords;
	texcoords.reserve(vertices.size());
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		float x = vertices[i].x / width + 0.5f,
			  y = vertices[i].y / height + 0.5f;
		texcoords.push_back(sm::vec2(x, y));
	}

	m_mesh_data = Triangles::Create(vertices, texcoords, triangles);
}

PointsMesh::~PointsMesh()
{
	delete m_mesh_data;
}

}