#include "MeshShape.h"

namespace eterrain2d
{

MeshShape::MeshShape(const d2d::Image& image)
	: emesh::Shape(image)
{
}

void MeshShape::InsertTriangle(const d2d::Vector* vertices, const d2d::Vector* texcoords)
{
	emesh::Triangle* tri = new emesh::Triangle;
	for (int i = 0; i < 3; ++i) {
		tri->nodes[i] = new emesh::Node(vertices[i], texcoords[i]);
	}
	m_tris.push_back(tri);
}

}