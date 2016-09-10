#include "NetworkMesh.h"
#include "NetworkShape.h"
#include "MeshTriangle.h"

#include <SM_Triangulation.h>

#include <map>

#include <assert.h>

namespace s2
{

NetworkMesh::NetworkMesh()
	: m_nw(NULL)
{
}

NetworkMesh::NetworkMesh(const NetworkMesh& nw)
	: Mesh(nw)
{
	if (nw.m_nw) {
		m_nw = nw.m_nw->Clone();
	} else {
		m_nw = NULL;
	}

	RefreshTriangles();

	// copy triangles
	assert(m_tris.size() == nw.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTriangle* src = nw.m_tris[i];
		MeshTriangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

NetworkMesh::NetworkMesh(const Symbol* base)
	: Mesh(base)
	, m_nw(NULL)
{
}

NetworkMesh::~NetworkMesh()
{
	if (m_nw) {
		delete m_nw;
	}
}

void NetworkMesh::SetShape(NetworkShape* shape)
{
	cu::RefCountObjAssign(m_nw, shape);
	RefreshTriangles();
}

void NetworkMesh::RefreshTriangles()
{
	ClearTriangles();

	std::vector<sm::vec2> tris;
	GetTriangulation(tris);

	LoadFromTriangulation(tris);
}

void NetworkMesh::GetTriangulation(std::vector<sm::vec2>& tris)
{
	if (m_nw) {
		sm::triangulate_points(m_nw->GetVertices(), m_nw->GetInnerVertices(), tris);
	}
}

void NetworkMesh::LoadFromTriangulation(const std::vector<sm::vec2>& tris)
{
	std::map<sm::vec2, MeshNode*, sm::Vector2Cmp> map2node;
	MeshNode null;
	for (int i = 0, n = tris.size(); i < n; ++i)
		map2node.insert(std::make_pair(tris[i], &null));

	for (int i = 0, n = tris.size() / 3, ptr = 0; i < n; ++i)
	{
		MeshTriangle* tri = new MeshTriangle;
		for (int j = 0; j < 3; ++j)
		{
			std::map<sm::vec2, MeshNode*, sm::Vector2Cmp>::iterator itr 
				= map2node.find(tris[ptr++]);
			assert(itr != map2node.end());
			if (itr->second == &null) {
				itr->second = new MeshNode(itr->first, m_width, m_height);
			} else {
				itr->second->AddReference();
			}
			tri->nodes[j] = itr->second;
		}
		m_tris.push_back(tri);
	}
}

//void NetworkMesh::GetRegionBound(std::vector<sm::vec2>& bound) const
//{
// 	if (m_use_region) {
// 		const sm::rect& r = m_region.rect;
// 		bound.push_back(sm::vec2(r.xmin, r.ymin));
// 		bound.push_back(sm::vec2(r.xmin, r.ymax));
// 		bound.push_back(sm::vec2(r.xmax, r.ymax));
// 		bound.push_back(sm::vec2(r.xmax, r.ymin));
// 	} else {
// 		std::copy(m_region.nodes.begin(), m_region.nodes.end(), back_inserter(bound));
// 	}
//}

}