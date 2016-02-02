#include "Mesh.h"
#include "Triangle.h"
#include "config.h"

#include <ee/JsonSerializer.h>
#include <ee/Triangulation.h>

#include <easyshape.h>

#include <assert.h>

namespace emesh
{

Mesh::Mesh(bool use_region)
	: m_use_region(use_region)
{
}

Mesh::Mesh(const Mesh& mesh)
	: EditShape(mesh)
	, m_region(mesh.m_region)
	, m_use_region(mesh.m_use_region)
{
	RefreshTriangles();

	// copy triangles
	assert(m_tris.size() == mesh.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* src = mesh.m_tris[i];
		Triangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

Mesh::Mesh(const ee::Image& image, bool initBound, bool use_region)
	: EditShape(image)
	, m_use_region(use_region)
{
	if (initBound)
	{
		float hw = m_width * 0.5f,
			  hh = m_height * 0.5f;
		m_region.rect.Combine(ee::Vector(hw, hh));
		m_region.rect.Combine(ee::Vector(-hw, -hh));
		RefreshTriangles();
	}
}

Mesh* Mesh::Clone() const
{
	return new Mesh(*this);
}

void Mesh::Load(const Json::Value& value)
{
	ee::Rect& r = m_region.rect;
	r.xmin = static_cast<float>(value["bound"]["xmin"].asDouble());
	r.xmax = static_cast<float>(value["bound"]["xmax"].asDouble());
	r.ymin = static_cast<float>(value["bound"]["ymin"].asDouble());
	r.ymax = static_cast<float>(value["bound"]["ymax"].asDouble());

	Json::Value loops_val = value["loops"];
	int i = 0;
	Json::Value loop_val = loops_val[i++];
	while (!loop_val.isNull()) {
		std::vector<ee::Vector> nodes;
		ee::JsonSerializer::Load(loop_val, nodes);
		eshape::ChainShape* shape = new eshape::ChainShape(nodes, true);
		m_region.loops.push_back(shape);
		loop_val = loops_val[i++];
	}

	RefreshTriangles();

#ifndef OPEN_MESH_INV
	if (!value["triangles"].isNull()) {
		LoadTriangles(value["triangles"]);
	}
#else
	if (!value["triangles"].isNull()) {
		LoadTriangles(value["triangles"]);
	}
	// load inverse mesh, need pair each node
	else if (!value["triangles_new"].isNull()) {
		std::vector<ee::Vector> transform_ori, transform_new;
		ee::JsonSerializer::Load(value["triangles"], transform_ori);
		ee::JsonSerializer::Load(value["triangles_new"], transform_new);

		int itr = 0;
		for (int i = 0, n = m_tris.size(); i < n; ++i)
		{
			Triangle* tri = m_tris[i];
			for (int i = 0; i < 3; ++i) 
			{
				Node* n = tri->nodes[i];

				for (int j = 0, m = transform_new.size(); j < m; ++j) {
					if (n->ori_xy == transform_new[j]) {
						n->xy = transform_ori[j];
						break;
					}
				}
			}
		}
	}
#endif
}

void Mesh::Store(Json::Value& value) const
{
	value["type"] = GetType();

	value["bound"]["xmin"] = m_region.rect.xmin;
	value["bound"]["xmax"] = m_region.rect.xmax;
	value["bound"]["ymin"] = m_region.rect.ymin;
	value["bound"]["ymax"] = m_region.rect.ymax;

#ifndef OPEN_MESH_INV
	Json::Value& loops_val = value["loops"];
	for (int i = 0, n = m_region.loops.size(); i < n; ++i) {
		const eshape::ChainShape* loop = m_region.loops[i];
		ee::JsonSerializer::Store(loop->GetVertices(), loops_val[i]);
	}

	StoreTriangles(value["triangles"]);
#else
	Json::Value& loops_val = value["loops"];
	for (int i = 0, n = m_region.loops.size(); i < n; ++i) {
		const eshape::ChainShape* loop = m_region.loops[i];
		const std::vector<ee::Vector>& src = loop->GetVertices();
		std::vector<ee::Vector> dst;
		for (int i = 0, n = src.size(); i < n; ++i) {
			bool find = false;
			for (int j = 0, m = m_tris.size(); j < m && !find; ++j) {
				Triangle* tri = m_tris[j];
				for (int k = 0; k < 3 && !find; ++k) {
					Node* n = tri->nodes[k];
					if (src[i] == n->ori_xy) {
						dst.push_back(n->xy);
						find = true;
					}
				}
			}
		}
		ee::JsonSerializer::Store(dst, loops_val[i]);
	}

	std::vector<ee::Vector> transform;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform.push_back(tri->nodes[i]->ori_xy);
	}
	ee::JsonSerializer::Store(transform, value["triangles"]);

	std::vector<ee::Vector> transform_new;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform_new.push_back(tri->nodes[i]->xy);
	}
	ee::JsonSerializer::Store(transform_new, value["triangles_new"]);
#endif
}

void Mesh::OffsetUV(float dx, float dy)
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

void Mesh::Refresh()
{
	RefreshTriangles();
}

void Mesh::TraverseShape(ee::Visitor& visitor) const
{
	for (int i = 0, n = m_region.loops.size(); i < n; ++i)
	{
		eshape::ChainShape* shape 
			= const_cast<eshape::ChainShape*>(m_region.loops[i]);

		bool has_next;
		visitor.Visit(shape, has_next);
		if (!has_next) {
			break;
		}
	}
}

bool Mesh::RemoveShape(ee::Shape* shape)
{
	bool ret = false;
	for (int i = 0, n = m_region.loops.size(); i < n; ++i)
	{
		ee::Shape* loop = m_region.loops[i];
		if (loop == shape) {
			loop->Release();
			m_region.loops.erase(m_region.loops.begin() + i);
			ret = true;
			break;
		}
	}
	return ret;
}

bool Mesh::InsertShape(ee::Shape* shape)
{
	eshape::ChainShape* loop = dynamic_cast<eshape::ChainShape*>(shape);
	if (loop) {
		loop->Retain();
		m_region.loops.push_back(loop);
		return true;
	} else {
		return false;
	}
}

bool Mesh::ClearShape()
{
	bool ret = !m_region.loops.empty();
	for (int i = 0, n = m_region.loops.size(); i < n; ++i) {
		m_region.loops[i]->Release();
	}
	m_region.loops.clear();
	return ret;
}

void Mesh::Reset()
{
	m_uv_offset.Set(0, 0);
	RefreshTriangles();
}

void Mesh::Clear()
{
	//	m_region.nodes.clear();
	RefreshTriangles();
}

void Mesh::RefreshTriangles()
{
	ClearTriangles();

	std::vector<ee::Vector> tris;
	GetTriangulation(tris);
	
	LoadFromTriangulation(tris);
}
 
void Mesh::GetTriangulation(std::vector<ee::Vector>& tris)
{
	if (m_use_region)
	{
		std::vector<ee::Vector> bound;
		const ee::Rect& r = m_region.rect;
		bound.push_back(ee::Vector(r.xmin, r.ymin));
		bound.push_back(ee::Vector(r.xmin, r.ymax));
		bound.push_back(ee::Vector(r.xmax, r.ymax));
		bound.push_back(ee::Vector(r.xmax, r.ymin));

		std::vector<ee::Vector> points;
		for (int i = 0, n = m_region.loops.size(); i < n; ++i)
		{
			const eshape::ChainShape* chain = m_region.loops[i];
			const std::vector<ee::Vector>& loop = chain->GetVertices();
			std::copy(loop.begin(), loop.end(), back_inserter(points));
		}
		ee::Triangulation::Points(bound, points, tris);
	}
	else
	{
 		for (int i = 0, n = m_region.loops.size(); i < n; ++i)
 		{
 			const eshape::ChainShape* chain = m_region.loops[i];
 			const std::vector<ee::Vector>& loop = chain->GetVertices();
 			ee::Triangulation::Normal(loop, tris);
 		}

// 		for (int i = 0, n = m_region.loops.size(); i < n; ++i) {
// 			const std::vector<ee::Vector>& loop0 = m_region.loops[i]->getVertices();
// 			for (int j = 0; j < n; ++j) {
// 				if (i != j) {
// 					const std::vector<ee::Vector>& loop1 = m_region.loops[j]->getVertices();
// 					if (ee::Math2D::isPointInArea(loop1[0], loop0)) {
// 
// // 						std::vector<std::vector<ee::Vector> > holes;
// // 						holes.push_back(loop1);
// // 						ee::Triangulation::Holes(loop0, holes, tris);
// 
// 						ee::Triangulation::HolesNew(loop0, loop1, tris);
// 					}
// 				}
// 			}
// 		}
	}
}

void Mesh::LoadFromTriangulation(const std::vector<ee::Vector>& tris)
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
			if (itr->second == &null)
				itr->second = new Node(itr->first, m_width, m_height);
			tri->nodes[j] = itr->second;
		}
		m_tris.push_back(tri);
	}
}

void Mesh::GetRegionBound(std::vector<ee::Vector>& bound) const
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

//void Mesh::getLinesCutByUVBounds(std::vector<ee::Vector>& lines)
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