#include "Mesh.h"
#include "Triangle.h"

namespace emesh
{

Mesh::Mesh(bool use_region)
	: m_use_region(use_region)
{
}

Mesh::Mesh(const Mesh& mesh)
	: Shape(mesh)
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
	: Shape(image)
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

Mesh* Mesh::clone() const
{
	return new Mesh(*this);
}

void Mesh::Insert(const d2d::Vector& p)
{
	if (d2d::Math::isPointInRect(p, m_region.rect))
	{
		m_region.nodes.push_back(p);
		RefreshTriangles();
	}
}

void Mesh::Remove(const d2d::Vector& p)
{
	if (d2d::Math::isPointInRect(p, m_region.rect))
	{
		std::vector<d2d::Vector>::iterator itr
			= m_region.nodes.begin();
		for ( ; itr != m_region.nodes.end(); ++itr)
		{
			if (d2d::Math::getDistance(*itr, p) < m_node_radius)
			{
				m_region.nodes.erase(itr);
				RefreshTriangles();
				break;
			}
		}
	}
}

d2d::Vector* Mesh::Find(const d2d::Vector& p)
{
	for (int i = 0, m = m_region.nodes.size(); i < m; ++i)
		if (d2d::Math::getDistance(m_region.nodes[i], p) < m_node_radius)
			return &m_region.nodes[i];
	return NULL;
}

void Mesh::Move(d2d::Vector* src, const d2d::Vector& dst)
{
	if (!src) return;

	if (d2d::Math::isPointInRect(dst, m_region.rect))
	{
		d2d::Vector d = dst;

		// absorb node to region
		const d2d::Rect& r = m_region.rect;
		if (fabs(d.x - r.xMin) < m_node_radius) {
			d.x = r.xMin;
		}
		if (fabs(d.x - r.xMax) < m_node_radius) {
			d.x = r.xMax;
		}
		if (fabs(d.y - r.yMin) < m_node_radius) {
			d.y = r.yMin;
		}
		if (fabs(d.y - r.yMax) < m_node_radius) {
			d.y = r.yMax;
		}

		*src = d;

		// remove corner rrom nodes
		std::vector<d2d::Vector>::iterator itr = m_region.nodes.begin();
		for ( ; itr != m_region.nodes.end(); )
		{
			if ((itr->x == r.xMin || itr->x == r.xMax)
				&& (itr->y == r.yMin || itr->y == r.yMax)) {
					itr = m_region.nodes.erase(itr);
			} else {
				++itr;
			}
		}

		RefreshTriangles();
	}
}

void Mesh::Reset()
{
	m_uv_offset.set(0, 0);
	RefreshTriangles();
}

void Mesh::Clear()
{
	m_region.nodes.clear();
	RefreshTriangles();
}

void Mesh::OffsetUV(float dx, float dy)
{
	// update uv base
	m_uv_offset.x += dx;
	m_uv_offset.y += dy;
	m_uv_offset.x = m_uv_offset.x - std::floor(m_uv_offset.x);
	m_uv_offset.y = m_uv_offset.y - std::floor(m_uv_offset.y);

	// insert node 
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

	std::vector<d2d::Vector> bound;
	GetRegionBound(bound);
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
		float y = -height*0.5f + height*m_uv_offset.y;
		for (int i = 0, n = m_tris.size(); i < n; ++i) {
			Triangle* tri = m_tris[i];
			for (int i = 0; i < 3; ++i) {
				Node* curr = tri->nodes[i];
				Node* next = tri->nodes[(i+1)%3];
				if ((y > curr->ori_xy.y && y < next->ori_xy.y || 
					y < curr->ori_xy.y && y > next->ori_xy.y) &&
					(curr->ori_xy != curr->xy || next->ori_xy != next->xy)) {
// 						d2d::Vector from, to;
// 						from.y = to.y = y;
// 						from.x = d2d::Math::findXOnSeg(curr->ori_xy, next->ori_xy, y);
// 						to.x = d2d::Math::findXOnSeg(curr->xy, next->xy, y);

						d2d::Vector pos(0.0f, y);
						d2d::Vector from, to;
						d2d::Math::getFootOfPerpendicular(curr->ori_xy, next->ori_xy, pos, &from);

						float p = d2d::Math::getDistance(pos, curr->ori_xy) / d2d::Math::getDistance(curr->ori_xy, next->ori_xy);
						to = curr->xy + (next->xy - curr->xy) * p;

						trans_list.push_back(std::make_pair(from, to));
				}
			}
		}
	}

	// create triangles separate
	ClearTriangles();
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

	// set uv between textures
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

void Mesh::Load(const Json::Value& value)
{
	d2d::Rect& r = m_region.rect;
	r.xMin = value["bound"]["xmin"].asDouble();
	r.xMax = value["bound"]["xmax"].asDouble();
	r.yMin = value["bound"]["ymin"].asDouble();
	r.yMax = value["bound"]["ymax"].asDouble();

	d2d::JsonTools::load(value["nodes"], m_region.nodes);

	RefreshTriangles();

	LoadTriangles(value["triangles"]);
}

void Mesh::Store(Json::Value& value) const
{
	value["type"] = GetType();

	value["bound"]["xmin"] = m_region.rect.xMin;
	value["bound"]["xmax"] = m_region.rect.xMax;
	value["bound"]["ymin"] = m_region.rect.yMin;
	value["bound"]["ymax"] = m_region.rect.yMax;

	d2d::JsonTools::store(m_region.nodes, value["nodes"]);

	StoreTriangles(value["triangles"]);
}

void Mesh::RefreshTriangles()
{
	ClearTriangles();

	if (!m_use_region && m_region.nodes.size() < 3) {
		return;
	}

	std::vector<d2d::Vector> bound;
	GetRegionBound(bound);
	std::vector<d2d::Vector> tris;
	if (m_use_region) {
		d2d::Triangulation::points(bound, m_region.nodes, tris);
	} else {
		std::vector<d2d::Vector> empty;
		d2d::Triangulation::points(bound, empty, tris);
	}
	
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
	if (m_use_region) {
		const d2d::Rect& r = m_region.rect;
		bound.push_back(d2d::Vector(r.xMin, r.yMin));
		bound.push_back(d2d::Vector(r.xMin, r.yMax));
		bound.push_back(d2d::Vector(r.xMax, r.yMax));
		bound.push_back(d2d::Vector(r.xMax, r.yMin));
	} else {
		std::copy(m_region.nodes.begin(), m_region.nodes.end(), back_inserter(bound));
	}
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