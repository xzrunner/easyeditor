#include "MeshShape.h"

namespace eterrain2d
{

MeshShape::MeshShape(const ee::Image& image)
	: emesh::Shape(image)
{
}

void MeshShape::InsertTriangle(const ee::Vector* vertices, 
							   const ee::Vector* texcoords,
							   const std::vector<ee::Vector>& bound)
{
	emesh::Triangle* tri = new emesh::Triangle;
	for (int i = 0; i < 3; ++i) {
		emesh::Node* n = new emesh::Node(vertices[i], texcoords[i]);
		
		for (int j = 0, m = bound.size(); j < m; ++j) {
			if (ee::Math2D::IsPointOnPolyline(n->ori_xy, bound)) {
				n->ud = (void*)&MeshShape::IS_BOUND;
			} else {
				n->ud = (void*)&MeshShape::NOT_BOUND;
			}
		}

		tri->nodes[i] = n;
	}
	m_tris.push_back(tri);
}

}