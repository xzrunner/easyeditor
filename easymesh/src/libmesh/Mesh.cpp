#include "Mesh.h"
#include "Triangle.h"

namespace emesh
{

Mesh::Mesh()
	: m_texid(0)
	, m_width(0)
	, m_height(0)
{
}

Mesh::Mesh(const Mesh& mesh)
	: m_texid(mesh.m_texid)
	, m_width(mesh.m_width)
	, m_height(mesh.m_height)
	, m_region(mesh.m_region)
{
	refreshTriangles();
	copyTriangles(mesh);
}

Mesh::Mesh(const d2d::Image& image, bool initBound)
{
	m_texid = image.textureID();

	m_width = image.getRegion().xLength();
	m_height = image.getRegion().yLength();

	if (initBound)
	{
		float hw = m_width * 0.5f,
			  hh = m_height * 0.5f;
		m_region.rect.combine(d2d::Vector(hw, hh));
		m_region.rect.combine(d2d::Vector(-hw, -hh));
		refreshTriangles();
	}
}

Mesh::~Mesh()
{
	clearTriangles();
}

Mesh* Mesh::clone() const
{
	return new Mesh(*this);
}

void Mesh::insert(const d2d::Vector& p)
{
	if (d2d::Math::isPointInRect(p, m_region.rect))
	{
		m_region.nodes.push_back(p);
		refreshTriangles();
	}
}

void Mesh::remove(const d2d::Vector& p)
{
	if (d2d::Math::isPointInRect(p, m_region.rect))
	{
		std::vector<d2d::Vector>::iterator itr
			= m_region.nodes.begin();
		for ( ; itr != m_region.nodes.end(); ++itr)
		{
			if (d2d::Math::getDistance(*itr, p) < Node::RADIUS)
			{
				m_region.nodes.erase(itr);
				refreshTriangles();
				break;
			}
		}
	}
}

d2d::Vector* Mesh::find(const d2d::Vector& p)
{
// 	for (int i = 0, m = m_region.bound.size(); i < m; ++i)
// 		if (d2d::Math::getDistance(m_region.bound[i], p) < Node::RADIUS)
// 			return &m_region.bound[i];
	for (int i = 0, m = m_region.nodes.size(); i < m; ++i)
		if (d2d::Math::getDistance(m_region.nodes[i], p) < Node::RADIUS)
			return &m_region.nodes[i];
	return NULL;
}

void Mesh::move(d2d::Vector* src, const d2d::Vector& dst)
{
	if (!src) return;

	if (d2d::Math::isPointInRect(dst, m_region.rect))
	{
		d2d::Vector d = dst;
		absorbNodeToRegion(d);
		*src = d;
		removeCornerFromNodes();
		refreshTriangles();
	}
}

Node* Mesh::queryNode(const d2d::Vector& p)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			if (d2d::Math::getDistance(tri->nodes[j]->xy, p) < Node::RADIUS)
				return tri->nodes[j];
		}
	}
	return NULL;
}

void Mesh::queryNode(const d2d::Rect& r, std::vector<Node*>& nodes)
{
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			if (d2d::Math::isPointInRect(tri->nodes[j]->xy, r))
				nodes.push_back(tri->nodes[j]);
		}
	}
}

void Mesh::drawInfoUV(const d2d::Screen& scr) const
{
	std::set<d2d::Vector, d2d::VectorCmp> unique;
	std::vector<d2d::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * m_width;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * m_height;
			unique.insert(tmp[i]);
		}
		d2d::PrimitiveDraw::drawPolyline(scr, tmp, d2d::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<d2d::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	d2d::PrimitiveDraw::drawCircles(scr, nodes, Node::RADIUS, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Mesh::drawInfoXY(const d2d::Screen& scr) const
{
	std::set<d2d::Vector, d2d::VectorCmp> unique;
	std::vector<d2d::Vector> tmp(3);
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			unique.insert(tmp[i]);
		}
		d2d::PrimitiveDraw::drawPolyline(scr, tmp, d2d::Colorf(0.8f, 0.2f, 0.4f, 0.5f), true);
	}
	std::vector<d2d::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	d2d::PrimitiveDraw::drawCircles(scr, nodes, Node::RADIUS, true, 2, d2d::Colorf(0.4f, 0.2f, 0.8f, 0.5f));
}

void Mesh::drawTexture(const d2d::Screen& scr) const
{
	std::vector<d2d::Vector> vertices, texcoords;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i)
		{
			texcoords.push_back(tri->nodes[i]->uv);
			vertices.push_back(tri->nodes[i]->xy);
		}
	}

	d2d::PrimitiveDraw::drawTriangles(scr, m_texid, vertices, texcoords);
}

void Mesh::tween(const Mesh& begin, const Mesh& end, float process)
{
	assert(m_tris.size() == begin.m_tris.size() && m_tris.size() == end.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			const d2d::Vector& p0 = begin.m_tris[i]->nodes[j]->xy;
			const d2d::Vector& p1 = end.m_tris[i]->nodes[j]->xy;
			m_tris[i]->nodes[j]->xy = p0 + (p1 - p0) * process;
		}
	}
}

void Mesh::reset()
{
	m_uv_offset.set(0, 0);
	refreshTriangles();
}

void Mesh::clear()
{
	m_region.nodes.clear();
	refreshTriangles();
}

void Mesh::resetUVOffset(float dx, float dy)
{
	m_uv_offset.x += dx;
	m_uv_offset.y += dy;
	m_uv_offset.x = m_uv_offset.x - std::floor(m_uv_offset.x);
	m_uv_offset.y = m_uv_offset.y - std::floor(m_uv_offset.y);

	refreshTrianglesWithUV();

	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		d2d::Rect r;
		r.makeInfinite();
		for (int i = 0; i < 3; ++i) {
			r.combine(tri->nodes[i]->uv);
		}

		for (int i = 0; i < 3; ++i) 
		{
			Node* n = tri->nodes[i];
			float x = n->uv.x - m_uv_offset.x;
			float y = n->uv.y - m_uv_offset.y;
			x = x - std::floor(x);
			y = y - std::floor(y);
			if (fabs(x - 0) < 0.0001f && n->uv.x == r.xMax) {
				x = 1;
			}
			if (fabs(x - 1) < 0.0001f && n->uv.x == r.xMin) {
				x = 0;
			}
			if (fabs(y - 0) < 0.0001f && n->uv.y == r.yMax) {
				y = 1;
			}
			if (fabs(y - 1) < 0.0001f && n->uv.y == r.yMin) {
				y = 0;
			}
			n->uv.x = x;
			n->uv.y = y;
		}
	}
}

void Mesh::getRegionBound(std::vector<d2d::Vector>& bound) const
{
	const d2d::Rect& r = m_region.rect;
	bound.push_back(d2d::Vector(r.xMin, r.yMin));
	bound.push_back(d2d::Vector(r.xMin, r.yMax));
	bound.push_back(d2d::Vector(r.xMax, r.yMax));
	bound.push_back(d2d::Vector(r.xMax, r.yMin));
}

void Mesh::refreshTriangles()
{
	clearTriangles();

	std::vector<d2d::Vector> bound;
	getRegionBound(bound);
	std::vector<d2d::Vector> tris;
	d2d::Triangulation::points(bound, m_region.nodes, tris);
	loadTriangles(tris);
}

void Mesh::getLinesCutByUVBounds(std::vector<d2d::Vector>& lines)
{
	// hori
	if (m_uv_offset.y != 0)
	{
		int height = m_region.rect.yLength();
		std::set<d2d::Vector, d2d::VectorCmpX> nodes;
		float y = -height*0.5f + height*m_uv_offset.y;
		for (int i = 0, n = m_tris.size(); i < n; ++i)
		{
			Triangle* tri = m_tris[i];
			for (int i = 0; i < 3; ++i) {
				Node* sn = tri->nodes[i];
				Node* en = tri->nodes[(i+1) % 3];
				if (sn->xy.y == en->xy.y) {
					continue;
				}
				float x = d2d::Math::findXOnSeg(sn->xy, en->xy, y);
				if (x > sn->xy.x && x < en->xy.x) {
					nodes.insert(d2d::Vector(x, y));
				}
			}
		}
		assert(nodes.size() % 2 == 0);
		copy(nodes.begin(), nodes.end(), back_inserter(lines));
	}
	// vert
	if (m_uv_offset.x != 0)
	{
		int width = m_region.rect.xLength();
		std::set<d2d::Vector, d2d::VectorCmpY> nodes;
		float x = -width*0.5f + width*m_uv_offset.x;
		for (int i = 0, n = m_tris.size(); i < n; ++i)
		{
			Triangle* tri = m_tris[i];
			for (int i = 0; i < 3; ++i) {
				Node* sn = tri->nodes[i];
				Node* en = tri->nodes[(i+1) % 3];
				if (sn->xy.x == en->xy.x) {
					continue;
				}
				float y = d2d::Math::findXOnSeg(sn->xy, en->xy, x);
				if (y > sn->xy.y && y < en->xy.y) {
					nodes.insert(d2d::Vector(x, y));
				}
			}
		}
		assert(nodes.size() % 2 == 0);
		copy(nodes.begin(), nodes.end(), back_inserter(lines));
	}
}

void Mesh::refreshTrianglesWithUV()
{
	//// get bounds
	//std::set<d2d::Vector, d2d::VectorCmp> bounds;
	//for (int i = 0, n = m_region.bound.size(); i < n; ++i) {
	//	bounds.insert(m_region.bound[i]);
	//}
	//std::set<d2d::Vector, d2d::VectorCmp> unique;
	//for (int i = 0, n = m_tris.size(); i < n; ++i)
	//{
	//	Triangle* tri = m_tris[i];
	//	for (int i = 0; i < 3; ++i) {
	//		if (bounds.find(tri->nodes[i]->xy) == bounds.end()) {
	//			unique.insert(tri->nodes[i]->xy);
	//		}
	//	}
	//}
	//std::vector<d2d::Vector> nodes;
	//copy(unique.begin(), unique.end(), back_inserter(nodes));

	// triangulation
	int width = m_region.rect.xLength();
	int height = m_region.rect.yLength();

	const float MAX = 99999;
	std::vector<d2d::Vector> tris;
	std::vector<d2d::Vector> lines;
	if (m_uv_offset.x != 0)
	{
		float x = -width*0.5f + width*m_uv_offset.x;
		lines.push_back(d2d::Vector(x, -MAX));
		lines.push_back(d2d::Vector(x, MAX));
	}
	if (m_uv_offset.y != 0)
	{
		float y = -height*0.5f + height*m_uv_offset.y;
		lines.push_back(d2d::Vector(-MAX, y));
		lines.push_back(d2d::Vector(MAX, y));
	}
	//std::vector<d2d::Vector> tris;
	//std::vector<d2d::Vector> lines;
	//getLinesCutByUVBounds(lines);
	std::vector<d2d::Vector> bound;
	getRegionBound(bound);
	d2d::Triangulation::pointsAndLines(bound, m_region.nodes, lines, tris);

	std::vector<std::pair<d2d::Vector, d2d::Vector> > trans_list;
	std::set<Node*> nodes;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			nodes.insert(tri->nodes[i]);
		}
	}
	std::set<Node*>::iterator itr = nodes.begin();
	for ( ; itr != nodes.end(); ++itr)
	{
		Node* n = *itr;
		if (n->xy != n->ori_xy) {
			trans_list.push_back(std::make_pair(n->ori_xy, n->xy));
		}
	}

	// uv bound to trans_list
	if (m_uv_offset.y != 0)
	{
// 		float hw = width * 0.5f;
// 		float y = -height*0.5f + height*m_uv_offset.y;
// 		for (int i = 0, n = m_tris.size(); i < n; ++i) {
// 			Triangle* tri = m_tris[i];
// 			for (int i = 0; i < 3; ++i) {
// 				Node* curr = tri->nodes[i];
// 				Node* next = tri->nodes[(i+1)%3];
// 				if (((y > curr->ori_xy.y && y < next->ori_xy.y) || (y < curr->ori_xy.y && y > next->ori_xy.y)) &&
// 					fabs(curr->ori_xy.x - hw) < 0.0001f && fabs(next->ori_xy.x - hw) < 0.0001f) {
// 						d2d::Vector from = (curr->ori_xy + next->ori_xy) * 0.5f;
// 						d2d::Vector to = (curr->xy + next->xy) * 0.5f;
// 
// 						trans_list.push_back(std::make_pair(n->ori_xy, n->xy));
// 				}
// 			}
// 		}

		float y = -height*0.5f + height*m_uv_offset.y;
		for (int i = 0, n = m_tris.size(); i < n; ++i) {
			Triangle* tri = m_tris[i];
			for (int i = 0; i < 3; ++i) {
				Node* curr = tri->nodes[i];
				Node* next = tri->nodes[(i+1)%3];
				if ((y > curr->ori_xy.y && y < next->ori_xy.y || 
					y < curr->ori_xy.y && y > next->ori_xy.y) &&
					(curr->ori_xy != curr->xy || next->ori_xy != next->xy)) {
						d2d::Vector from, to;
						from.y = to.y = y;
						from.x = d2d::Math::findXOnSeg(curr->ori_xy, next->ori_xy, y);
						to.x = d2d::Math::findXOnSeg(curr->xy, next->xy, y);
						trans_list.push_back(std::make_pair(from, to));
				}
			}
		}
	}


	// create tris
	clearTriangles();
	for (int i = 0, n = tris.size() / 3, ptr = 0; i < n; ++i)
	{
		Triangle* tri = new Triangle;
		for (int j = 0; j < 3; ++j) {
			Node* n = new Node(tris[ptr++], m_width, m_height);
			for (int i = 0, m = trans_list.size(); i < m; ++i)
			{
				float dis = d2d::Math::getDistanceSquare(n->ori_xy, trans_list[i].first);
				if (dis < 0.01) {
					n->xy = trans_list[i].second;
					break;
				}
			}
			tri->nodes[j] = n;
		}
		m_tris.push_back(tri);
	}


	//// for uv bounds
	//if (m_uv_offset.x != 0)
	//{
	//	float x = -width*0.5f + width*m_uv_offset.x;
	//	std::map<d2d::Vector, std::vector<std::pair<Node*, Node*> >, d2d::VectorCmp> connected;
	//	for (int i = 0, n = m_tris.size(); i < n; ++i)
	//	{
	//		for (int j = 0; j < 3; ++j)
	//		{
	//			Node* curr = m_tris[i]->nodes[j];
	//			if (curr->xy.x == x) {
	//				Node* next = m_tris[i]->nodes[(j+1)%3];
	//				if (next->xy.x != x) {
	//					std::map<d2d::Vector, std::vector<std::pair<Node*, Node*> >, d2d::VectorCmp>::iterator itr 
	//						= connected.find(curr->xy);
	//					if (itr != connected.end()) {
	//						itr->second.push_back(std::make_pair(curr, next));
	//					} else {
	//						std::vector<std::pair<Node*, Node*> > nodes;
	//						nodes.push_back(std::make_pair(curr, next));
	//						connected.insert(std::make_pair(curr->xy, nodes));
	//					}
	//				}
	//			}
	//		}
	//	}
	//	assert(connected.size() == 0 || connected.size() == 2);
	//}
	//if (m_uv_offset.y != 0)
	//{
	//	float y = -height*0.5f + height*m_uv_offset.y;
	//	std::map<d2d::Vector, std::vector<std::pair<Node*, Node*> >, d2d::VectorCmp> connected;
	//	for (int i = 0, n = m_tris.size(); i < n; ++i)
	//	{
	//		for (int j = 0; j < 3; ++j)
	//		{
	//			Node* curr = m_tris[i]->nodes[j];
	//			if (curr->xy.y == y) {
	//				Node* next = m_tris[i]->nodes[(j+1)%3];
	//				if (next->xy.y != y) {
	//					std::map<d2d::Vector, std::vector<std::pair<Node*, Node*> >, d2d::VectorCmp>::iterator itr 
	//						= connected.find(curr->xy);
	//					if (itr != connected.end()) {
	//						itr->second.push_back(std::make_pair(curr, next));
	//					} else {
	//						std::vector<std::pair<Node*, Node*> > nodes;
	//						nodes.push_back(std::make_pair(curr, next));
	//						connected.insert(std::make_pair(curr->xy, nodes));
	//					}
	//				}
	//			}
	//		}
	//	}
	//	assert(connected.size() == 0 || connected.size() == 2);

	//	std::map<d2d::Vector, std::vector<std::pair<Node*, Node*> >, d2d::VectorCmp>::iterator itr 
	//		= connected.begin();
	//	for ( ; itr != connected.end(); ++itr)
	//	{
	//		float cx = 0;
	//		int size = itr->second.size();
	//		for (int i = 0; i < size; ++i) {
	//			cx += itr->second[i].second->xy.x;
	//		}
	//		cx /= size;
	//		for (int i = 0; i < size; ++i) {
	//			itr->second[i].second->xy.x = cx;
	//		}
	//	}
	//}
}

void Mesh::clearTriangles()
{
	for_each(m_tris.begin(), m_tris.end(), DeletePointerFunctor<Triangle>());
	m_tris.clear();
}

void Mesh::loadTriangles(const std::vector<d2d::Vector>& tris)
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

void Mesh::copyTriangles(const Mesh& mesh)
{
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

void Mesh::absorbNodeToRegion(d2d::Vector& node)
{
	const d2d::Rect& r = m_region.rect;
	if (fabs(node.x - r.xMin) < Node::RADIUS) {
		node.x = r.xMin;
	}
	if (fabs(node.x - r.xMax) < Node::RADIUS) {
		node.x = r.xMax;
	}
	if (fabs(node.y - r.yMin) < Node::RADIUS) {
		node.y = r.yMin;
	}
	if (fabs(node.y - r.yMax) < Node::RADIUS) {
		node.y = r.yMax;
	}
}

void Mesh::removeCornerFromNodes()
{
	const d2d::Rect& r = m_region.rect;
	std::vector<d2d::Vector>::iterator itr = m_region.nodes.begin();
	for ( ; itr != m_region.nodes.end(); )
	{
		if ((itr->x == r.xMin || itr->x == r.xMax)
			&& (itr->y == r.yMin || itr->y == r.yMax)) {
			itr = m_region.nodes.erase(itr);
		} else {
			++itr;
		}

		// 		int i = 0, n = m_region.bound.size();
		// 		for (; i < n; ++i)
		// 		{
		// 			float dis = d2d::Math::getDistanceSquare(*itr, m_region.bound[i]);
		// 			if (dis < Node::RADIUS) {
		// 				itr = m_region.nodes.erase(itr);
		// 				break;
		// 			}
		// 		}
		// 		if (i == n) {
		// 			++itr;
		// 		}
	}
}

}