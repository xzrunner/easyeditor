#include "Network.h"
#include "NetworkShape.h"

#include <easyshape.h>

#include <sprite2/MeshTriangle.h>
#include <sprite2/NetworkShape.h>
#include <sprite2/MeshTransform2.h>
#include <gum/JsonSerializer.h>
#include <gum/MeshIO.h>

#include <assert.h>

namespace emesh
{

Network::Network()
{
}

Network::Network(const Network& mesh)
	: s2::Mesh(mesh)
	, Mesh(mesh)
	, s2::NetworkMesh(mesh)
{
}

Network::Network(const ee::Symbol* base)
	: s2::Mesh(base)
	, Mesh(base)
	, s2::NetworkMesh(base)
{
}

//Network* Network::Clone() const
//{
//	return new Network(*this);
//}

void Network::Load(const Json::Value& value)
{
	if (m_shape) {
		delete m_shape;
	}

	std::vector<sm::vec2> outline;
	gum::JsonSerializer::Load(value["shape"]["outline"], outline);
	m_shape = new NetworkShape(outline, m_node_radius);

	std::vector<sm::vec2> inner;
	gum::JsonSerializer::Load(value["shape"]["inner"], inner);
	m_shape->SetInnerVertices(inner);

	RefreshTriangles();

	s2::MeshTransform2 trans;
	gum::MeshIO::Load(value, trans, *this);
	trans.StoreToMesh(this);
}

void Network::Store(Json::Value& value) const
{
	value["type"] = GetTypeName();

	if (!m_shape) {
		return;
	}

	dynamic_cast<NetworkShape*>(m_shape)->StoreToFile(value, "");

	s2::MeshTransform2 trans;
	gum::MeshIO::Load(value, trans, *this);
	trans.StoreToMesh(this);
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
	//	std::vector<sm::vec2> tris;
	//	std::vector<sm::vec2> lines;
	//	if (m_uv_offset.x != 0)
	//	{
	//		float x = -width*0.5f + width*m_uv_offset.x;
	//		lines.push_back(sm::vec2(x, -MAX));
	//		lines.push_back(sm::vec2(x, MAX));
	//	}
	//	if (m_uv_offset.y != 0)
	//	{
	//		float y = -height*0.5f + height*m_uv_offset.y;
	//		lines.push_back(sm::vec2(-MAX, y));
	//		lines.push_back(sm::vec2(MAX, y));
	//	}
	//
	//	std::vector<sm::vec2> bound;
	//	GetRegionBound(bound);
	//	ee::Triangulation::pointsAndLines(bound, m_region.nodes, lines, tris);
	//
	//	std::vector<std::pair<sm::vec2, sm::vec2> > trans_list;
	//	std::set<Node*> nodes;
	//	for (int i = 0, n = m_tris.size(); i < n; ++i)
	//	{
	//		s2::MeshTriangle* tri = m_tris[i];
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
	//			s2::MeshTriangle* tri = m_tris[i];
	//			for (int i = 0; i < 3; ++i) {
	//				Node* curr = tri->nodes[i];
	//				Node* next = tri->nodes[(i+1)%3];
	//				if ((y > curr->ori_xy.y && y < next->ori_xy.y || 
	//					y < curr->ori_xy.y && y > next->ori_xy.y) &&
	//					(curr->ori_xy != curr->xy || next->ori_xy != next->xy)) {
	//// 						sm::vec2 from, to;
	//// 						from.y = to.y = y;
	//// 						from.x = ee::Math2D::findXOnSeg(curr->ori_xy, next->ori_xy, y);
	//// 						to.x = ee::Math2D::findXOnSeg(curr->xy, next->xy, y);
	//
	//						sm::vec2 pos(0.0f, y);
	//						sm::vec2 from, to;
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
	//		s2::MeshTriangle* tri = new s2::MeshTriangle;
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
	//		s2::MeshTriangle* tri = m_tris[i];
	//		sm::rect r;
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

void Network::TraverseMesh(ee::Visitor<ee::Shape>& visitor) const
{
	if (m_shape) {
		bool has_next;
		visitor.Visit(dynamic_cast<ee::Shape*>(m_shape), has_next);
	}
}

bool Network::RemoveMesh(ee::Shape* shape)
{
	if (shape == dynamic_cast<NetworkShape*>(m_shape)) {
		delete m_shape, m_shape = NULL;		
		return true;
	} else {
		return false;
	}
}

bool Network::InsertMesh(ee::Shape* shape)
{
	eshape::ChainShape* loop = dynamic_cast<eshape::ChainShape*>(shape);
	if (m_shape) {
		delete m_shape;
	}
	m_shape = new NetworkShape(loop->GetVertices(), m_node_radius);
	return true;
}

bool Network::ClearMesh()
{
	if (m_shape) {
		delete m_shape, m_shape = NULL;
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

bool Network::InsertInner(const sm::vec2& pos)
{
	if (!m_shape) {
		return false;
	} else {
		return dynamic_cast<NetworkShape*>(m_shape)->InsertInner(pos);
	}
}

bool Network::RemoveInner(const sm::vec2& pos)
{
	if (!m_shape) {
		return false;
	} else {
		return dynamic_cast<NetworkShape*>(m_shape)->RemoveInner(pos);
	}
}

sm::vec2* Network::QueryInner(const sm::vec2& pos)
{
	if (!m_shape) {
		return NULL;
	} else {
		return dynamic_cast<NetworkShape*>(m_shape)->QueryInner(pos);
	}
}
 
//void Network::getLinesCutByUVBounds(std::vector<sm::vec2>& lines)
//{
//	// hori
//	if (m_uv_offset.y != 0)
//	{
//		int height = m_region.rect.Height();
//		std::set<sm::vec2, ee::VectorCmpX> nodes;
//		float y = -height*0.5f + height*m_uv_offset.y;
//		for (int i = 0, n = m_tris.size(); i < n; ++i)
//		{
//			s2::MeshTriangle* tri = m_tris[i];
//			for (int i = 0; i < 3; ++i) {
//				Node* sn = tri->nodes[i];
//				Node* en = tri->nodes[(i+1) % 3];
//				if (sn->xy.y == en->xy.y) {
//					continue;
//				}
//				float x = ee::Math2D::findXOnSeg(sn->xy, en->xy, y);
//				if (x > sn->xy.x && x < en->xy.x) {
//					nodes.insert(sm::vec2(x, y));
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
//		std::set<sm::vec2, ee::VectorCmpY> nodes;
//		float x = -width*0.5f + width*m_uv_offset.x;
//		for (int i = 0, n = m_tris.size(); i < n; ++i)
//		{
//			s2::MeshTriangle* tri = m_tris[i];
//			for (int i = 0; i < 3; ++i) {
//				Node* sn = tri->nodes[i];
//				Node* en = tri->nodes[(i+1) % 3];
//				if (sn->xy.x == en->xy.x) {
//					continue;
//				}
//				float y = ee::Math2D::findXOnSeg(sn->xy, en->xy, x);
//				if (y > sn->xy.y && y < en->xy.y) {
//					nodes.insert(sm::vec2(x, y));
//				}
//			}
//		}
//		assert(nodes.size() % 2 == 0);
//		copy(nodes.begin(), nodes.end(), back_inserter(lines));
//	}
//}

}