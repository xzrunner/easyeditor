#include "Strip.h"
#include "Triangle.h"

namespace emesh
{

Strip::Strip()
{
}

Strip::Strip(const Strip& strip)
	: Shape(strip)
	, m_left_nodes(strip.m_left_nodes)
	, m_right_nodes(strip.m_right_nodes)
{
	RefreshTriangles();
	CopyTriangles(strip);
}

Strip::Strip(const d2d::Image& image)
	: Shape(image)
{
	InitBound();
}

Strip* Strip::clone() const
{
	return new Strip(*this);
}

void Strip::Insert(const d2d::Vector& p)
{
	assert(m_left_nodes.size() >= 2 && m_right_nodes.size() >= 2);

	d2d::Vector foot;
	if (d2d::Math::getFootOfPerpendicular(m_left_nodes.front(), m_left_nodes.back(), p, &foot) == -1
		|| d2d::Math::getFootOfPerpendicular(m_right_nodes.front(), m_right_nodes.back(), p, &foot)) {
		return;
	}

	InsertToEdge(m_left_nodes, p);
	InsertToEdge(m_right_nodes, p);

	RefreshTriangles();
}

void Strip::Remove(const d2d::Vector& p)
{
	assert(m_left_nodes.size() >= 2 && m_right_nodes.size() >= 2);

	for (int i = 1, n = m_left_nodes.size() - 1; i < n; ++i)
	{
		if (d2d::Math::getDistance(m_left_nodes[i], p) < Node::RADIUS)
		{
			m_left_nodes.erase(m_left_nodes.begin() + i);
			m_right_nodes.erase(m_right_nodes.begin() + i);
			RefreshTriangles();
			return;
		}
	}
	for (int i = 1, n = m_right_nodes.size() - 1; i < n; ++i)
	{
		if (d2d::Math::getDistance(m_right_nodes[i], p) < Node::RADIUS)
		{
			m_left_nodes.erase(m_left_nodes.begin() + i);
			m_right_nodes.erase(m_right_nodes.begin() + i);
			RefreshTriangles();
			return;
		}
	}
}

d2d::Vector* Strip::Find(const d2d::Vector& p)
{
	assert(m_left_nodes.size() >= 2 && m_right_nodes.size() >= 2);

	for (int i = 1, n = m_left_nodes.size() - 1; i < n; ++i) {
		if (d2d::Math::getDistance(m_left_nodes[i], p) < Node::RADIUS) {
			return &m_left_nodes[i];
		}
	}
	for (int i = 1, n = m_right_nodes.size() - 1; i < n; ++i) {
		if (d2d::Math::getDistance(m_right_nodes[i], p) < Node::RADIUS) {
			return &m_right_nodes[i];
		}
	}
	return NULL;
}

void Strip::Move(d2d::Vector* src, const d2d::Vector& dst)
{
	assert(m_left_nodes.size() >= 2 && m_right_nodes.size() >= 2);

	if (!src) return;

	src->y = dst.y;

	std::sort(m_left_nodes.begin(), m_left_nodes.end(), d2d::VectorCmpY());
	std::sort(m_right_nodes.begin(), m_right_nodes.end(), d2d::VectorCmpY());

	RefreshTriangles();
}

void Strip::Reset()
{
//	m_uv_offset.set(0, 0);
	RefreshTriangles();
}

void Strip::Clear()
{
	InitBound();
	RefreshTriangles();
}

void Strip::ResetUVOffset(float dx, float dy)
{
}

void Strip::InitBound()
{
	m_left_nodes.clear();
	m_right_nodes.clear();

	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	m_left_nodes.push_back(d2d::Vector(-hw, -hh));
	m_left_nodes.push_back(d2d::Vector(-hw,  hh));
	m_right_nodes.push_back(d2d::Vector(hw, -hh));
	m_right_nodes.push_back(d2d::Vector(hw,  hh));
}

void Strip::RefreshTriangles()
{
	ClearTriangles();

	assert(m_left_nodes.size() == m_right_nodes.size());
	std::vector<d2d::Vector> tris;
	for (int i = 0, n = m_left_nodes.size() - 1; i < n; ++i)
	{
		// todo
		tris.push_back(m_left_nodes[i]);
		tris.push_back(m_right_nodes[i]);
		tris.push_back(m_right_nodes[i+1]);
		tris.push_back(m_left_nodes[i]);
		tris.push_back(m_right_nodes[i+1]);
		tris.push_back(m_left_nodes[i+1]);

		//////////////////////////////////////////////////////////////////////////
		Triangle* tri0 = new Triangle;
		tri0->nodes[0] = new Node(m_left_nodes[i], m_width, m_height);
		tri0->nodes[1] = new Node(m_right_nodes[i], m_width, m_height);
		tri0->nodes[2] = new Node(m_right_nodes[i+1], m_width, m_height);
		m_tris.push_back(tri0);
		Triangle* tri1 = new Triangle;
		tri1->nodes[0] = new Node(m_left_nodes[i], m_width, m_height);
		tri1->nodes[1] = new Node(m_right_nodes[i+1], m_width, m_height);
		tri1->nodes[2] = new Node(m_left_nodes[i+1], m_width, m_height);
		m_tris.push_back(tri1);
	}
//	LoadTriangles(tris);
}

// void Strip::LoadTriangles(const std::vector<d2d::Vector>& tris)
// {
// 	
// }

void Strip::CopyTriangles(const Strip& strip)
{
	assert(m_tris.size() == strip.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		Triangle* src = strip.m_tris[i];
		Triangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j)
		{
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

void Strip::AbsorbNodeToRegion(d2d::Vector& node)
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	if (fabs(node.x - (-hw)) < Node::RADIUS) {
		node.x = -hw;
	}
	if (fabs(node.x - hw) < Node::RADIUS) {
		node.x = hw;
	}
	if (fabs(node.y - (-hh)) < Node::RADIUS) {
		node.y = -hh;
	}
	if (fabs(node.y - hh) < Node::RADIUS) {
		node.y = hh;
	}
}

void Strip::InsertToEdge(std::vector<d2d::Vector>& edge, const d2d::Vector& p)
{
	int idx_nearest = -1;
	float dis_nearest = FLT_MAX;
	d2d::Vector foot_nearest;
	for (int i = 0, n = edge.size() - 1; i < n; ++i)
	{
		d2d::Vector foot;
		int st = d2d::Math::getFootOfPerpendicular(edge[i], edge[i+1], p, &foot);
		if (st == -1) {
			continue;
		}
		float dis = d2d::Math::getDistanceSquare(foot, p);
		if (dis < dis_nearest) {
			idx_nearest = i;
			dis_nearest = dis;
			foot_nearest = foot;
		}
	}
	assert(idx_nearest != -1);
	edge.insert(edge.begin() + idx_nearest + 1, foot_nearest);
}

}