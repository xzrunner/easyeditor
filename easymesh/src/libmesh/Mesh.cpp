#include "Mesh.h"
#include "Triangle.h"
#include "config.h"

#include <easyshape.h>

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

Mesh::Mesh(const d2d::Image& image, bool initBound, bool use_region)
	: EditShape(image)
	, m_use_region(use_region)
{
	if (initBound)
	{
		float hw = m_width * 0.5f,
			  hh = m_height * 0.5f;
		m_region.rect.combine(d2d::Vector(hw, hh));
		m_region.rect.combine(d2d::Vector(-hw, -hh));
		RefreshTriangles();
	}
}

Mesh* Mesh::Clone() const
{
	return new Mesh(*this);
}

void Mesh::Load(const Json::Value& value)
{
	d2d::Rect& r = m_region.rect;
	r.xMin = value["bound"]["xmin"].asDouble();
	r.xMax = value["bound"]["xmax"].asDouble();
	r.yMin = value["bound"]["ymin"].asDouble();
	r.yMax = value["bound"]["ymax"].asDouble();

	Json::Value loops_val = value["loops"];
	int i = 0;
	Json::Value loop_val = loops_val[i++];
	while (!loop_val.isNull()) {
		std::vector<d2d::Vector> nodes;
		d2d::JsonIO::Load(loop_val, nodes);
		libshape::ChainShape* shape = new libshape::ChainShape(nodes, true);
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
		std::vector<d2d::Vector> transform_ori, transform_new;
		d2d::JsonIO::Load(value["triangles"], transform_ori);
		d2d::JsonIO::Load(value["triangles_new"], transform_new);

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

	value["bound"]["xmin"] = m_region.rect.xMin;
	value["bound"]["xmax"] = m_region.rect.xMax;
	value["bound"]["ymin"] = m_region.rect.yMin;
	value["bound"]["ymax"] = m_region.rect.yMax;

#ifndef OPEN_MESH_INV
	Json::Value& loops_val = value["loops"];
	for (int i = 0, n = m_region.loops.size(); i < n; ++i) {
		const libshape::ChainShape* loop = m_region.loops[i];
		d2d::JsonIO::Store(loop->GetVertices(), loops_val[i]);
	}

	StoreTriangles(value["triangles"]);
#else
	Json::Value& loops_val = value["loops"];
	for (int i = 0, n = m_region.loops.size(); i < n; ++i) {
		const libshape::ChainShape* loop = m_region.loops[i];
		const std::vector<d2d::Vector>& src = loop->GetVertices();
		std::vector<d2d::Vector> dst;
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
		d2d::JsonIO::Store(dst, loops_val[i]);
	}

	std::vector<d2d::Vector> transform;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform.push_back(tri->nodes[i]->ori_xy);
	}
	d2d::JsonIO::Store(transform, value["triangles"]);

	std::vector<d2d::Vector> transform_new;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform_new.push_back(tri->nodes[i]->xy);
	}
	d2d::JsonIO::Store(transform_new, value["triangles_new"]);
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
	//	int width = m_region.rect.xLength();
	//	int height = m_region.rect.yLength();
	//
	//	const float MAX = 99999;
	//	std::vector<d2d::Vector> tris;
	//	std::vector<d2d::Vector> lines;
	//	if (m_uv_offset.x != 0)
	//	{
	//		float x = -width*0.5f + width*m_uv_offset.x;
	//		lines.push_back(d2d::Vector(x, -MAX));
	//		lines.push_back(d2d::Vector(x, MAX));
	//	}
	//	if (m_uv_offset.y != 0)
	//	{
	//		float y = -height*0.5f + height*m_uv_offset.y;
	//		lines.push_back(d2d::Vector(-MAX, y));
	//		lines.push_back(d2d::Vector(MAX, y));
	//	}
	//
	//	std::vector<d2d::Vector> bound;
	//	GetRegionBound(bound);
	//	d2d::Triangulation::pointsAndLines(bound, m_region.nodes, lines, tris);
	//
	//	std::vector<std::pair<d2d::Vector, d2d::Vector> > trans_list;
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
	//// 						d2d::Vector from, to;
	//// 						from.y = to.y = y;
	//// 						from.x = d2d::Math::findXOnSeg(curr->ori_xy, next->ori_xy, y);
	//// 						to.x = d2d::Math::findXOnSeg(curr->xy, next->xy, y);
	//
	//						d2d::Vector pos(0.0f, y);
	//						d2d::Vector from, to;
	//						d2d::Math::getFootOfPerpendicular(curr->ori_xy, next->ori_xy, pos, &from);
	//
	//						float p = d2d::Math::getDistance(pos, curr->ori_xy) / d2d::Math::getDistance(curr->ori_xy, next->ori_xy);
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
	//				float dis = d2d::Math::getDistanceSquare(n->ori_xy, trans_list[i].first);
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
	//		d2d::Rect r;
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
	//			if (fabs(x - 0) < 0.0001f && n->uv.x == r.xMax) {
	//				x = 1;
	//			}
	//			if (fabs(x - 1) < 0.0001f && n->uv.x == r.xMin) {
	//				x = 0;
	//			}
	//			if (fabs(y - 0) < 0.0001f && n->uv.y == r.yMax) {
	//				y = 1;
	//			}
	//			if (fabs(y - 1) < 0.0001f && n->uv.y == r.yMin) {
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

void Mesh::TraverseShape(d2d::IVisitor& visitor) const
{
	for (int i = 0, n = m_region.loops.size(); i < n; ++i)
	{
		libshape::ChainShape* shape 
			= const_cast<libshape::ChainShape*>(m_region.loops[i]);

		bool has_next;
		visitor.Visit(shape, has_next);
		if (!has_next) {
			break;
		}
	}
}

bool Mesh::RemoveShape(d2d::IShape* shape)
{
	bool ret = false;
	for (int i = 0, n = m_region.loops.size(); i < n; ++i)
	{
		d2d::IShape* loop = m_region.loops[i];
		if (loop == shape) {
			loop->Release();
			m_region.loops.erase(m_region.loops.begin() + i);
			ret = true;
			break;
		}
	}
	return ret;
}

bool Mesh::InsertShape(d2d::IShape* shape)
{
	libshape::ChainShape* loop = dynamic_cast<libshape::ChainShape*>(shape);
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
	m_uv_offset.set(0, 0);
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

	std::vector<d2d::Vector> tris;
	GetTriangulation(tris);
	
	LoadFromTriangulation(tris);
}
 
void Mesh::GetTriangulation(std::vector<d2d::Vector>& tris)
{
	if (m_use_region)
	{
		std::vector<d2d::Vector> bound;
		const d2d::Rect& r = m_region.rect;
		bound.push_back(d2d::Vector(r.xMin, r.yMin));
		bound.push_back(d2d::Vector(r.xMin, r.yMax));
		bound.push_back(d2d::Vector(r.xMax, r.yMax));
		bound.push_back(d2d::Vector(r.xMax, r.yMin));

		std::vector<d2d::Vector> points;
		for (int i = 0, n = m_region.loops.size(); i < n; ++i)
		{
			const libshape::ChainShape* chain = m_region.loops[i];
			const std::vector<d2d::Vector>& loop = chain->GetVertices();
			std::copy(loop.begin(), loop.end(), back_inserter(points));
		}
		d2d::Triangulation::points(bound, points, tris);
	}
	else
	{
 		for (int i = 0, n = m_region.loops.size(); i < n; ++i)
 		{
 			const libshape::ChainShape* chain = m_region.loops[i];
 			const std::vector<d2d::Vector>& loop = chain->GetVertices();
 			d2d::Triangulation::normal(loop, tris);
 		}

// 		for (int i = 0, n = m_region.loops.size(); i < n; ++i) {
// 			const std::vector<d2d::Vector>& loop0 = m_region.loops[i]->getVertices();
// 			for (int j = 0; j < n; ++j) {
// 				if (i != j) {
// 					const std::vector<d2d::Vector>& loop1 = m_region.loops[j]->getVertices();
// 					if (d2d::Math::isPointInArea(loop1[0], loop0)) {
// 
// // 						std::vector<std::vector<d2d::Vector> > holes;
// // 						holes.push_back(loop1);
// // 						d2d::Triangulation::Holes(loop0, holes, tris);
// 
// 						d2d::Triangulation::HolesNew(loop0, loop1, tris);
// 					}
// 				}
// 			}
// 		}
	}
}

void Mesh::LoadFromTriangulation(const std::vector<d2d::Vector>& tris)
{
	std::map<d2d::Vector, Node*, d2d::VectorCmp> map2Node;
	Node null;
	for (int i = 0, n = tris.size(); i < n; ++i)
		map2Node.insert(std::make_pair(tris[i], &null));

	for (int i = 0, n = tris.size() / 3, ptr = 0; i < n; ++i)
	{
		Triangle* tri = new Triangle;
		for (int j = 0; j < 3; ++j)
		{
			std::map<d2d::Vector, Node*, d2d::VectorCmp>::iterator itr 
				= map2Node.find(tris[ptr++]);
			assert(itr != map2Node.end());
			if (itr->second == &null)
				itr->second = new Node(itr->first, m_width, m_height);
			tri->nodes[j] = itr->second;
		}
		m_tris.push_back(tri);
	}
}

void Mesh::GetRegionBound(std::vector<d2d::Vector>& bound) const
{
// 	if (m_use_region) {
// 		const d2d::Rect& r = m_region.rect;
// 		bound.push_back(d2d::Vector(r.xMin, r.yMin));
// 		bound.push_back(d2d::Vector(r.xMin, r.yMax));
// 		bound.push_back(d2d::Vector(r.xMax, r.yMax));
// 		bound.push_back(d2d::Vector(r.xMax, r.yMin));
// 	} else {
// 		std::copy(m_region.nodes.begin(), m_region.nodes.end(), back_inserter(bound));
// 	}
}

//void Mesh::getLinesCutByUVBounds(std::vector<d2d::Vector>& lines)
//{
//	// hori
//	if (m_uv_offset.y != 0)
//	{
//		int height = m_region.rect.yLength();
//		std::set<d2d::Vector, d2d::VectorCmpX> nodes;
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
//				float x = d2d::Math::findXOnSeg(sn->xy, en->xy, y);
//				if (x > sn->xy.x && x < en->xy.x) {
//					nodes.insert(d2d::Vector(x, y));
//				}
//			}
//		}
//		assert(nodes.size() % 2 == 0);
//		copy(nodes.begin(), nodes.end(), back_inserter(lines));
//	}
//	// vert
//	if (m_uv_offset.x != 0)
//	{
//		int width = m_region.rect.xLength();
//		std::set<d2d::Vector, d2d::VectorCmpY> nodes;
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
//				float y = d2d::Math::findXOnSeg(sn->xy, en->xy, x);
//				if (y > sn->xy.y && y < en->xy.y) {
//					nodes.insert(d2d::Vector(x, y));
//				}
//			}
//		}
//		assert(nodes.size() % 2 == 0);
//		copy(nodes.begin(), nodes.end(), back_inserter(lines));
//	}
//}

}