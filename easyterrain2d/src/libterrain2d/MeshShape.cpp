#include "MeshShape.h"

#include <ee/Math2D.h>

#include <SM_Test.h>

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
// 	s2::MeshTriangle* tri = new s2::MeshTriangle;
// 	for (int i = 0; i < 3; ++i) {
// 		s2::MeshNode* n = new s2::MeshNode(vertices[i], texcoords[i]);
// 		
// 		for (int j = 0, m = bound.size(); j < m; ++j) {
// 			if (sm::is_point_intersect_polyline(n->ori_xy, bound)) {
// 				n->ud = (void*)&MeshShape::IS_BOUND;
// 			} else {
// 				n->ud = (void*)&MeshShape::NOT_BOUND;
// 			}
// 		}
// 
// 		tri->nodes[i] = n;
// 	}
// 	m_tris.push_back(tri);
}

}