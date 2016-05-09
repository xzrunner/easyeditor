#include "MeshShape.h"

#include <ee/Math2D.h>

namespace eterrain2d
{

MeshShape::MeshShape(const ee::Symbol* base)
	: emesh::Mesh(base)
{
}

void MeshShape::InsertTriangle(const sm::vec2* vertices, 
							   const sm::vec2* texcoords,
							   const std::vector<sm::vec2>& bound)
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