#include "Network.h"
#include "Triangle.h"
#include "config.h"
#include "NetworkShape.h"
#include "MeshTrans.h"

#include <ee/JsonSerializer.h>
#include <ee/Triangulation.h>

#include <easyshape.h>

#include <assert.h>

namespace emesh
{

Network::Network()
	: m_nw(NULL)
{
}

Network::Network(const Network& nw)
	: EditableMesh(nw)
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
		Triangle* src = nw.m_tris[i];
		Triangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

Network::Network(const ee::Image& image)
	: EditableMesh(image)
	, m_nw(NULL)
{
}

Network::~Network()
{
	if (m_nw) {
		delete m_nw;
	}
}

Network* Network::Clone() const
{
	return new Network(*this);
}

void Network::Load(const Json::Value& value)
{
	if (m_nw) {
		delete m_nw;
	}

	std::vector<ee::Vector> outline;
	ee::JsonSerializer::Load(value["shape"]["outline"], outline);
	m_nw = new NetworkShape(new eshape::ChainShape(outline, true), m_node_radius);

	std::vector<ee::Vector> inner;
	ee::JsonSerializer::Load(value["shape"]["inner"], inner);
	m_nw->SetInnerVertices(inner);

	RefreshTriangles();

	MeshTrans trans;
	trans.Load(value);
	trans.StoreToMesh(this);
}

void Network::Store(Json::Value& value) const
{
	value["type"] = GetType();

	if (!m_nw) {
		return;
	}

	m_nw->StoreToFile(value, "");

	MeshTrans trans;
	trans.LoadFromMesh(this);
	trans.Store(value);
}

void Network::OffsetUV(float dx, float dy)
{
	//	// update uv base
	//	m_uv_offset.x += dx;
	//	m_uv_offset.y += dy;
	//	m_uv_offset.x = m_uv_offset.x - std::floor(m_uv_offset.x);
	//	m_uv_offset.y = m_uv_offset.y - std::floor(m_uv_offset.y);
	//
	//	// insert node 
	//	int width = m_region.rect.Width();
	//	int height = m_region.rect.Height();
	//
	//	const float MAX = 99999;
	//	std::vector<ee::Vector> tris;
	//	std::vector<ee::Vector> lines;
	//	if (m_uv_offset.x != 0)
	//	{
	//		float x = -width*0.5f + width*m_uv_offset.x;
	//		lines.push_back(ee::Vector(x, -MAX));
	//		lines.push_back(ee::Vector(x, MAX));
	//	}
	//	if (m_uv_offset.y != 0)
	//	{
	//		float y = -height*0.5f + height*m_uv_offset.y;
	//		lines.push_back(ee::Vector(-MAX, y));
	//		lines.push_back(ee::Vector(MAX, y));
	//	}
	//
	//	std::vector<ee::Vector> bound;
	//	GetRegionBound(bound);
	//	ee::Triangulation::pointsAndLines(bound, m_region.nodes, lines, tris);
	//
	//	std::vector<std::pair<ee::Vector, ee::Vector> > trans_list;
	//	std::set<Node*> nodes;
	//	for (int i = 0, n = m_tris.size(); i < n; ++i)
	//	{
	//		Triangle* tri = m_tris[i];
	//		for (int i = 0; i < 3; ++i) {
	//			nodes.insert(tri->nodes[i]);
	//		}
	//	}
	//	std::set<Node*>::iterator itr = nodes.begin();
	//	for ( ; itr != nodes.end(); ++itr)
	//	{
	//		Node* n = *itr;
	//		if (n->xy != n->ori_xy) {
	//			trans_list.push_back(std::make_pair(n->ori_xy, n->xy));
	//		}
	//	}
	//
	//	// uv bound to trans_list
	//	if (m_uv_offset.y != 0)
	//	{
	//		float y = -height*0.5f + height*m_uv_offset.y;
	//		for (int i = 0, n = m_tris.size(); i < n; ++i) {
	//			Triangle* tri = m_tris[i];
	//			for (int i = 0; i < 3; ++i) {
	//				Node* curr = tri->nodes[i];
	//				Node* next = tri->nodes[(i+1)%3];
	//				if ((y > curr->ori_xy.y && y < next->ori_xy.y || 
	//					y < curr->ori_xy.y && y > next->ori_xy.y) &&
	//					(curr->ori_xy != curr->xy || next->ori_xy != next->xy)) {
	//// 						ee::Vector from, to;
	//// 						from.y = to.y = y;
	//// 						from.x = ee::Math2D::findXOnSeg(curr->ori_xy, next->ori_xy, y);
	//// 						to.x = ee::Math2D::findXOnSeg(curr->xy, next->xy, y);
	//
	//						ee::Vector pos(0.0f, y);
	//						ee::Vector from, to;
	//						ee::Math2D::getFootOfPerpendicular(curr->ori_xy, next->ori_xy, pos, &from);
	//
	//						float p = ee::Math2D::getDistance(pos, curr->ori_xy) / ee::Math2D::getDistance(curr->ori_xy, next->ori_xy);
	//						to = curr->xy + (next->xy - curr->xy) * p;
	//
	//						trans_list.push_back(std::make_pair(from, to));
	//				}
	//			}
	//		}
	//	}
	//
	//	// create triangles separate
	//	ClearTriangles();
	//	for (int i = 0, n = tris.size() / 3, ptr = 0; i < n; ++i)
	//	{
	//		Triangle* tri = new Triangle;
	//		for (int j = 0; j < 3; ++j) {
	//			Node* n = new Node(tris[ptr++], m_width, m_height);
	//			for (int i = 0, m = trans_list.size(); i < m; ++i)
	//			{
	//				float dis = ee::Math2D::getDistanceSquare(n->ori_xy, trans_list[i].first);
	//				if (dis < 0.01) {
	//					n->xy = trans_list[i].second;
	//					break;
	//				}
	//			}
	//			tri->nodes[j] = n;
	//		}
	//		m_tris.push_back(tri);
	//	}
	//
	//	// set uv between textures
	//	for (int i = 0, n = m_tris.size(); i < n; ++i)
	//	{
	//		Triangle* tri = m_tris[i];
	//		ee::Rect r;
	//		r.makeInfinite();
	//		for (int i = 0; i < 3; ++i) {
	//			r.combine(tri->nodes[i]->uv);
	//		}
	//
	//		for (int i = 0; i < 3; ++i) 
	//		{
	//			Node* n = tri->nodes[i];
	//			float x = n->uv.x - m_uv_offset.x;
	//			float y = n->uv.y - m_uv_offset.y;
	//			x = x - std::floor(x);
	//			y = y - std::floor(y);
	//			if (fabs(x - 0) < 0.0001f && n->uv.x == r.xmax) {
	//				x = 1;
	//			}
	//			if (fabs(x - 1) < 0.0001f && n->uv.x == r.xmin) {
	//				x = 0;
	//			}
	//			if (fabs(y - 0) < 0.0001f && n->uv.y == r.ymax) {
	//				y = 1;
	//			}
	//			if (fabs(y - 1) < 0.0001f && n->uv.y == r.ymin) {
	//				y = 0;
	//			}
	//			n->uv.x = x;
	//			n->uv.y = y;
	//		}
	//	}
}

void Network::Refresh()
{
	RefreshTriangles();
}

void Network::TraverseMesh(ee::Visitor& visitor) const
{
	if (m_nw) {
		bool has_next;
		visitor.Visit(m_nw, has_next);
	}
}

bool Network::RemoveMesh(ee::Shape* mesh)
{
	if (mesh == m_nw) {
		delete m_nw, m_nw = NULL;		
		return true;
	} else {
		return false;
	}
}

bool Network::InsertMesh(ee::Shape* mesh)
{
	eshape::ChainShape* loop = dynamic_cast<eshape::ChainShape*>(mesh);
	if (m_nw) {
		delete m_nw;
	}
	m_nw = new NetworkShape(loop, m_node_radius);
	return true;
}

bool Network::ClearMesh()
{
	if (m_nw) {
		delete m_nw, m_nw = NULL;
		return true;
	} else {
		return false;
	}
}

void Network::Reset()
{
	m_uv_offset.Set(0, 0);
	RefreshTriangles();
}

void Network::Clear()
{
	//	m_region.nodes.clear();
	ClearTriangles();
}

bool Network::InsertInner(const ee::Vector& pos)
{
	if (!m_nw) {
		return false;
	} else {
		return m_nw->InsertInner(pos);
	}
}

bool Network::RemoveInner(const ee::Vector& pos)
{
	if (!m_nw) {
		return false;
	} else {
		return m_nw->RemoveInner(pos);
	}
}

ee::Vector* Network::QueryInner(const ee::Vector& pos)
{
	if (!m_nw) {
		return NULL;
	} else {
		return m_nw->QueryInner(pos);
	}
}

void Network::RefreshTriangles()
{
	ClearTriangles();

	std::vector<ee::Vector> tris;
	GetTriangulation(tris);
	
	LoadFromTriangulation(tris);
}
 
void Network::GetTriangulation(std::vector<ee::Vector>& tris)
{
	if (m_nw) {
		ee::Triangulation::Points(m_nw->GetVertices(), m_nw->GetInnerVertices(), tris);
	}
}

void Network::LoadFromTriangulation(const std::vector<ee::Vector>& tris)
{
	std::map<ee::Vector, Node*, ee::VectorCmp> map2Node;
	Node null;
	for (int i = 0, n = tris.size(); i < n; ++i)
		map2Node.insert(std::make_pair(tris[i], &null));

	for (int i = 0, n = tris.size() / 3, ptr = 0; i < n; ++i)
	{
		Triangle* tri = new Triangle;
		for (int j = 0; j < 3; ++j)
		{
			std::map<ee::Vector, Node*, ee::VectorCmp>::iterator itr 
				= map2Node.find(tris[ptr++]);
			assert(itr != map2Node.end());
			if (itr->second == &null) {
				itr->second = new Node(itr->first, m_width, m_height);
			} else {
				itr->second->Retain();
			}
			tri->nodes[j] = itr->second;
		}
		m_tris.push_back(tri);
	}
}

void Network::GetRegionBound(std::vector<ee::Vector>& bound) const
{
// 	if (m_use_region) {
// 		const ee::Rect& r = m_region.rect;
// 		bound.push_back(ee::Vector(r.xmin, r.ymin));
// 		bound.push_back(ee::Vector(r.xmin, r.ymax));
// 		bound.push_back(ee::Vector(r.xmax, r.ymax));
// 		bound.push_back(ee::Vector(r.xmax, r.ymin));
// 	} else {
// 		std::copy(m_region.nodes.begin(), m_region.nodes.end(), back_inserter(bound));
// 	}
}

//void Network::getLinesCutByUVBounds(std::vector<ee::Vector>& lines)
//{
//	// hori
//	if (m_uv_offset.y != 0)
//	{
//		int height = m_region.rect.Height();
//		std::set<ee::Vector, ee::VectorCmpX> nodes;
//		float y = -height*0.5f + height*m_uv_offset.y;
//		for (int i = 0, n = m_tris.size(); i < n; ++i)
//		{
//			Triangle* tri = m_tris[i];
//			for (int i = 0; i < 3; ++i) {
//				Node* sn = tri->nodes[i];
//				Node* en = tri->nodes[(i+1) % 3];
//				if (sn->xy.y == en->xy.y) {
//					continue;
//				}
//				float x = ee::Math2D::findXOnSeg(sn->xy, en->xy, y);
//				if (x > sn->xy.x && x < en->xy.x) {
//					nodes.insert(ee::Vector(x, y));
//				}
//			}
//		}
//		assert(nodes.size() % 2 == 0);
//		copy(nodes.begin(), nodes.end(), back_inserter(lines));
//	}
//	// vert
//	if (m_uv_offset.x != 0)
//	{
//		int width = m_region.rect.Width();
//		std::set<ee::Vector, ee::VectorCmpY> nodes;
//		float x = -width*0.5f + width*m_uv_offset.x;
//		for (int i = 0, n = m_tris.size(); i < n; ++i)
//		{
//			Triangle* tri = m_tris[i];
//			for (int i = 0; i < 3; ++i) {
//				Node* sn = tri->nodes[i];
//				Node* en = tri->nodes[(i+1) % 3];
//				if (sn->xy.x == en->xy.x) {
//					continue;
//				}
//				float y = ee::Math2D::findXOnSeg(sn->xy, en->xy, x);
//				if (y > sn->xy.y && y < en->xy.y) {
//					nodes.insert(ee::Vector(x, y));
//				}
//			}
//		}
//		assert(nodes.size() % 2 == 0);
//		copy(nodes.begin(), nodes.end(), back_inserter(lines));
//	}
//}

}