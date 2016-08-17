#include "DelaunayTriangulation.h"
#include "Math2D.h"
#include "Random.h"
#include "std_functor.h"

#include <SM_Test.h>
#include <SM_Calc.h>

#include <queue>

#include <assert.h>

namespace ee
{

DelaunayTriangulation::DelaunayTriangulation(const std::vector<sm::vec2>& src, bool bFixBound /*= true*/)
{
	if (src.size() >= 3)
	{
		InitSurrondTri(src);
		InitSrcNodes(src);
		BuildTriangulationStruct();
		SetAllDestTris(bFixBound);
	}
}

DelaunayTriangulation::~DelaunayTriangulation()
{
	for_each(m_nodes.begin(), m_nodes.end(), DeletePointerFunctor<Node>());

	std::set<Triangle*> tris;
	std::set<Edge*> edges;
	std::queue<Triangle*> buffer;

	tris.insert(m_root);
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Triangle* tri = buffer.front(); buffer.pop();
		if (!tri->m_leaf)
			for (size_t i = 0; i < tri->m_children.size(); ++i)
			{
				Triangle* child(tri->m_children[i]);
				std::pair<std::set<Triangle*>::iterator, bool> state
					= tris.insert(child);
				if (state.second)
				{
					buffer.push(child);
					edges.insert(child->m_e01.first);
					edges.insert(child->m_e12.first);
					edges.insert(child->m_e20.first);
				}
			}
	}

	for_each(tris.begin(), tris.end(), DeletePointerFunctor<Triangle>());
	for_each(edges.begin(), edges.end(), DeletePointerFunctor<Edge>());
}

void DelaunayTriangulation::GetAllTrisInRegion(std::vector<std::vector<sm::vec2> >& triBounds, 
											   const std::vector<sm::vec2>& region) const
{
	std::vector<Triangle*> tris = GetAllTris();
	for (size_t i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* src = tris[i];
		std::vector<sm::vec2> bound;
		src->GetNodesPos(bound);

		sm::vec2 p = Math2D::GetTriGravityCenter(bound[0], bound[1], bound[2]);
		if (sm::is_point_in_area(p, region))
			triBounds.push_back(bound);
	}
}

void DelaunayTriangulation::GetAllTrisInRegion(std::vector<sm::vec2>& triBounds, const std::vector<sm::vec2>& region) const
{
	std::vector<Triangle*> tris = GetAllTris();
	for (size_t i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* src = tris[i];
		std::vector<sm::vec2> bound;
		src->GetNodesPos(bound);

		sm::vec2 p = Math2D::GetTriGravityCenter(bound[0], bound[1], bound[2]);
		if (sm::is_point_in_area(p, region))
			copy(bound.begin(), bound.end(), back_inserter(triBounds));
	}
}

bool DelaunayTriangulation::InsertNode(const sm::vec2& p)
{
	int type = Math2D::CheckPosInTriangle(p, m_root->m_nodes[0]->m_pos, m_root->m_nodes[1]->m_pos, m_root->m_nodes[2]->m_pos);
	if (type == -1 || type == 3)
		return false;

	Node* n = new Node(p);
	std::pair<std::set<Node*, NodeCmp>::iterator, bool> state = m_nodes.insert(n);
	if (state.second)
	{
		if (InsertNode(n))
		{
			// todo: can update partially
			SetAllDestTris();

			return true;
		}
		else
			return false;
	}
	else
	{
		delete n;
		return false;
	}
}

void DelaunayTriangulation::DeleteNodes(const std::vector<sm::vec2>& pos)
{
	for (size_t i = 0; i < pos.size(); ++i)
	{
		Node n(pos[i]);
		std::set<Node*, NodeCmp>::iterator itr = m_nodes.find(&n);
		if (itr == m_nodes.end())
			continue;

		DeleteNode(*itr);
		delete *itr;
		m_nodes.erase(itr);
	}
	// todo: can update partially
	SetAllDestTris();
}

bool DelaunayTriangulation::IsBoundNode(const sm::vec2& n) const
{
	Node tmp(n);
	std::set<Node*, NodeCmp>::const_iterator itr = m_nodes.find(&tmp);
	if (itr == m_nodes.end())
		return false;
	else
		return (*itr)->IsMargin();
}

void DelaunayTriangulation::GetBoundLinePos(std::vector<std::vector<sm::vec2> >& bounds) const
{
	std::set<sm::vec2, PosDownCmp> leftPos;
	std::set<Node*, NodeCmp>::const_iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr)
		if (IsBoundNode((*itr)->m_pos))
			leftPos.insert((*itr)->m_pos);

	while (!leftPos.empty())
	{
		std::vector<sm::vec2> bound;
		bound.push_back(*leftPos.begin());
		leftPos.erase(leftPos.begin());
		while (true)
		{
			Node nCurr(bound.back());
			std::set<Node*, NodeCmp>::const_iterator ptrCurrNode = m_nodes.find(&nCurr);
			assert(ptrCurrNode != m_nodes.end());

			float minAngle = FLT_MAX;
			sm::vec2 nextPos;
			for (size_t i = 0; i < (*ptrCurrNode)->m_edges.size(); ++i)
			{
				Edge* e((*ptrCurrNode)->m_edges[i]);
				if (e->IsMargin())
				{
					sm::vec2 otherPos(e->m_start == *ptrCurrNode ? e->m_end->m_pos : e->m_start->m_pos);

					if (bound.size() >= 2 && otherPos == bound[bound.size() - 2])
						continue;

					float angle;
					if (bound.size() < 2)
						angle = sm::get_line_angle(bound[bound.size() - 1], otherPos);
					else
						angle = Math2D::GetAngle(bound[bound.size() - 1], bound[bound.size() - 2], otherPos);
					if (angle < minAngle)
					{
						minAngle = angle;
						nextPos = otherPos;
					}
				}
			}
			assert(minAngle != FLT_MAX);

			if (nextPos == bound.front())
				break;
			else
			{
				bound.push_back(nextPos);
				leftPos.erase(nextPos);
			}
		}
		assert(bound.size() >= 3);
		bounds.push_back(bound);
	}	
}

void DelaunayTriangulation::GetSurroundPos(const sm::vec2& pos, std::vector<sm::vec2>& surround) const
{
	Node tmp(pos);
	std::set<Node*, NodeCmp>::const_iterator itr = m_nodes.find(&tmp);
	if (itr != m_nodes.end())
	{
		Node* n(*itr);
		surround.reserve(n->m_edges.size());
		for (size_t i = 0; i < n->m_edges.size(); ++i)
		{
			Edge* e(n->m_edges[i]);
			surround.push_back(e->m_start == n ? e->m_end->m_pos : e->m_start->m_pos);
		}
	}
}

//
// class DelaunayTriangulation::Node
//

void DelaunayTriangulation::Node::GetSurroundTris(std::vector<const Triangle*>& tris) const
{
	for (size_t i = 0; i < m_edges.size(); ++i)
	{
		const Edge* e(m_edges[i]);
		tris.push_back(e->m_start == this ? e->LeftTri() : e->RightTri());
	}
}

bool DelaunayTriangulation::Node::IsMargin() const
{
	for (size_t i = 0; i < m_edges.size(); ++i)
		if (m_edges[i]->IsMargin())
			return true;
	return false;
}

//
// class DelaunayTriangulation::Edge
//

void DelaunayTriangulation::Edge::
	InsertNode(Node* n)
{
	Triangle *tLeftStart  = new Triangle, *tLeftEnd  = new Triangle,
		*tRightStart = new Triangle, *tRightEnd = new Triangle;
	Edge *eLeft  = new Edge(n, m_left.second, std::make_pair(tLeftStart, m_start), std::make_pair(tLeftEnd, m_end)),
		*eRight = new Edge(n, m_right.second, std::make_pair(tRightEnd, m_end), std::make_pair(tRightStart, m_start)),
		*eStart = new Edge(m_start, n, std::make_pair(tLeftStart, m_left.second), std::make_pair(tRightStart, m_right.second)),
		*eEnd   = new Edge(n, m_end, std::make_pair(tLeftEnd, m_left.second), std::make_pair(tRightEnd, m_right.second));

	std::pair<Edge*, bool> eLeftStart, eLeftEnd, eRightStart, eRightEnd;
	LeftTri()->GetNextTwoEdge(this, &eLeftEnd, &eLeftStart);
	RightTri()->GetNextTwoEdge(this, &eRightStart, &eRightEnd);

	tLeftStart->InitTri(n, m_left.second, m_start, std::make_pair(eLeft, true), eLeftStart, std::make_pair(eStart, true));
	tLeftEnd->InitTri(n, m_end, m_left.second, std::make_pair(eEnd, true), eLeftEnd, std::make_pair(eLeft, false));
	tRightStart->InitTri(n, m_start, m_right.second, std::make_pair(eStart, false), eRightStart, std::make_pair(eRight, false));
	tRightEnd->InitTri(n, m_right.second, m_end, std::make_pair(eRight, true), eRightEnd, std::make_pair(eEnd, false));

	if (eLeftStart.second)
		eLeftStart.first->ResetTri(std::make_pair(tLeftStart, n), true);
	else
		eLeftStart.first->ResetTri(std::make_pair(tLeftStart, n), false);
	if (eLeftEnd.second)
		eLeftEnd.first->ResetTri(std::make_pair(tLeftEnd, n), true);
	else
		eLeftEnd.first->ResetTri(std::make_pair(tLeftEnd, n), false);
	if (eRightStart.second)
		eRightStart.first->ResetTri(std::make_pair(tRightStart, n), true);
	else
		eRightStart.first->ResetTri(std::make_pair(tRightStart, n), false);
	if (eRightEnd.second)
		eRightEnd.first->ResetTri(std::make_pair(tRightEnd, n), true);
	else
		eRightEnd.first->ResetTri(std::make_pair(tRightEnd, n), false);

	LeftTri()->AddChildren(tLeftStart, tLeftEnd);
	RightTri()->AddChildren(tRightStart, tRightEnd);

	eLeftStart.first->LegalizeEdge(eLeftStart.second);
	eLeftEnd.first->LegalizeEdge(eLeftEnd.second);
	eRightStart.first->LegalizeEdge(eRightStart.second);
	eRightEnd.first->LegalizeEdge(eRightEnd.second);
}

void DelaunayTriangulation::Edge::
	LegalizeEdge(bool checkRightTri)
{
	if (m_start->m_fake && m_end->m_fake)
		return;

	sm::vec2 checkPos(m_left.second->m_pos), fixPos(m_right.second->m_pos);
	if (checkRightTri)
		std::swap(checkPos, fixPos);

	sm::vec2 center;
	Math2D::GetCircumcenter(m_start->m_pos, m_end->m_pos, fixPos, &center);
	if (sm::dis_pos_to_pos(checkPos, center) < sm::dis_pos_to_pos(fixPos, center))
	{
		assert(LeftTri()->m_leaf && RightTri()->m_leaf);

		if (checkRightTri) {
			Triangle *newTriLeft = new Triangle, *newTriRight = new Triangle;
			Edge* newEdge = new Edge(m_left.second, m_right.second, std::make_pair(newTriLeft, m_end), std::make_pair(newTriRight, m_start));

			std::pair<Edge*, bool> triLeftFix, triRightFix, triLeftCheck, triRightCheck;
			LeftTri()->GetNextTwoEdge(this, &triLeftFix, &triRightFix);
			RightTri()->GetNextTwoEdge(this, &triRightCheck, &triLeftCheck);

			if (triLeftFix.second)
				triLeftFix.first->ResetTri(std::make_pair(newTriLeft, m_right.second), true);
			else
				triLeftFix.first->ResetTri(std::make_pair(newTriLeft, m_right.second), false);
			if (triRightFix.second)
				triRightFix.first->ResetTri(std::make_pair(newTriRight, m_right.second), true);
			else
				triRightFix.first->ResetTri(std::make_pair(newTriRight, m_right.second), false);
			if (triLeftCheck.second)
				triLeftCheck.first->ResetTri(std::make_pair(newTriLeft, m_left.second), true);
			else
				triLeftCheck.first->ResetTri(std::make_pair(newTriLeft, m_left.second), false);
			if (triRightCheck.second)
				triRightCheck.first->ResetTri(std::make_pair(newTriRight, m_left.second), true);
			else
				triRightCheck.first->ResetTri(std::make_pair(newTriRight, m_left.second), false);

			newTriLeft->InitTri(m_left.second, m_right.second, m_end, std::make_pair(newEdge, true), triLeftCheck, triLeftFix);
			newTriRight->InitTri(m_right.second, m_left.second, m_start, std::make_pair(newEdge, false), triRightFix, triRightCheck);

			LeftTri()->AddChildren(newTriLeft, newTriRight);
			RightTri()->AddChildren(newTriLeft, newTriRight);

			triLeftCheck.first->LegalizeEdge(triLeftCheck.second);
			triRightCheck.first->LegalizeEdge(triRightCheck.second);
		}
		else {
			Triangle *newTriLeft = new Triangle, *newTriRight = new Triangle;
			Edge* newEdge = new Edge(m_right.second, m_left.second, std::make_pair(newTriLeft, m_start), std::make_pair(newTriRight, m_end));

			std::pair<Edge*, bool> triLeftFix, triRightFix, triLeftCheck, triRightCheck;
			RightTri()->GetNextTwoEdge(this, &triLeftFix, &triRightFix);
			LeftTri()->GetNextTwoEdge(this, &triRightCheck, &triLeftCheck);

			if (triLeftFix.second)
				triLeftFix.first->ResetTri(std::make_pair(newTriLeft, m_left.second), true);
			else
				triLeftFix.first->ResetTri(std::make_pair(newTriLeft, m_left.second), false);
			if (triRightFix.second)
				triRightFix.first->ResetTri(std::make_pair(newTriRight, m_left.second), true);
			else
				triRightFix.first->ResetTri(std::make_pair(newTriRight, m_left.second), false);
			if (triLeftCheck.second)
				triLeftCheck.first->ResetTri(std::make_pair(newTriLeft, m_right.second), true);
			else
				triLeftCheck.first->ResetTri(std::make_pair(newTriLeft, m_right.second), false);
			if (triRightCheck.second)
				triRightCheck.first->ResetTri(std::make_pair(newTriRight, m_right.second), true);
			else
				triRightCheck.first->ResetTri(std::make_pair(newTriRight, m_right.second), false);

			newTriLeft->InitTri(m_right.second, m_left.second, m_start, std::make_pair(newEdge, true), triLeftCheck, triLeftFix);
			newTriRight->InitTri(m_left.second, m_right.second, m_end, std::make_pair(newEdge, false), triRightFix, triRightCheck);

			LeftTri()->AddChildren(newTriLeft, newTriRight);
			RightTri()->AddChildren(newTriLeft, newTriRight);

			triLeftCheck.first->LegalizeEdge(triLeftCheck.second);
			triRightCheck.first->LegalizeEdge(triRightCheck.second);
		}
	}
}

float DelaunayTriangulation::Edge::Length() const
{
	return sm::dis_pos_to_pos(m_start->m_pos, m_end->m_pos);
}

//
// class DelaunayTriangulation::Triangle
//

DelaunayTriangulation::Triangle::Triangle()
	: m_leaf(true), m_removed(false)
{ 
	m_nodes[0] = m_nodes[1] = m_nodes[2] = NULL;
	m_e01.first = m_e12.first = m_e20.first = NULL;
}

void DelaunayTriangulation::Triangle::
	InitTri(Node* in0, Node* in1, Node* in2)
{
	m_nodes[0] = in0;
	m_nodes[1] = in1;
	m_nodes[2] = in2;
	m_e01 = std::make_pair(new Edge(m_nodes[0], m_nodes[1], std::make_pair(this, m_nodes[2])), true);
	m_e12 = std::make_pair(new Edge(m_nodes[1], m_nodes[2], std::make_pair(this, m_nodes[0])), true); 
	m_e20 = std::make_pair(new Edge(m_nodes[2], m_nodes[0], std::make_pair(this, m_nodes[1])), true);
	m_leaf = true;
}

void DelaunayTriangulation::Triangle::
	InsertNode(Node* n)
{
	Triangle *t01 = new Triangle, *t12 = new Triangle, *t20 = new Triangle;
	Edge *e0n = new Edge(m_nodes[0], n, std::make_pair(t20, m_nodes[2]), std::make_pair(t01, m_nodes[1])),
		*e1n = new Edge(m_nodes[1], n, std::make_pair(t01, m_nodes[0]), std::make_pair(t12, m_nodes[2])), 
		*e2n = new Edge(m_nodes[2], n, std::make_pair(t12, m_nodes[1]), std::make_pair(t20, m_nodes[0]));
	t01->InitTri(m_nodes[0], m_nodes[1], n, m_e01, std::make_pair(e1n, true), std::make_pair(e0n, false));
	t12->InitTri(m_nodes[1], m_nodes[2], n, m_e12, std::make_pair(e2n, true), std::make_pair(e1n, false));
	t20->InitTri(m_nodes[2], m_nodes[0], n, m_e20, std::make_pair(e0n, true), std::make_pair(e2n, false));

	if (m_e01.second)
		m_e01.first->ResetTri(std::make_pair(t01, n), true);
	else
		m_e01.first->ResetTri(std::make_pair(t01, n), false);
	if (m_e12.second)
		m_e12.first->ResetTri(std::make_pair(t12, n), true);
	else
		m_e12.first->ResetTri(std::make_pair(t12, n), false);
	if (m_e20.second)
		m_e20.first->ResetTri(std::make_pair(t20, n), true);
	else
		m_e20.first->ResetTri(std::make_pair(t20, n), false);

	AddChildren(t01, t12, t20);

	m_e01.first->LegalizeEdge(m_e01.second);
	m_e12.first->LegalizeEdge(m_e12.second);
	m_e20.first->LegalizeEdge(m_e20.second);
}

void DelaunayTriangulation::Triangle::
	InitTri(Node* pN0, Node* pN1, Node* pN2, const std::pair<Edge*, bool>& pE01, 
		const std::pair<Edge*, bool>& pE12, const std::pair<Edge*, bool>& pE20)
{
	m_nodes[0]  = pN0;
	m_nodes[1]  = pN1;
	m_nodes[2]  = pN2;
	m_e01 = pE01;
	m_e12 = pE12; 
	m_e20 = pE20;
	m_leaf = true;
};

void DelaunayTriangulation::Triangle::
	AddChildren(Triangle* t0, Triangle* t1)
{
	assert(m_leaf);
	m_leaf = false;
	t0->m_parent.push_back(this);
	t1->m_parent.push_back(this);
	m_children.push_back(t0);
	m_children.push_back(t1);
}

void DelaunayTriangulation::Triangle::
	AddChildren(Triangle* t0, Triangle* t1, Triangle* t2)
{
	assert(m_leaf);
	m_leaf = false;
	t0->m_parent.push_back(this);
	t1->m_parent.push_back(this);
	t2->m_parent.push_back(this);
	m_children.push_back(t0);
	m_children.push_back(t1);
	m_children.push_back(t2);
}

void DelaunayTriangulation::Triangle::
	GetNextTwoEdge(const Edge* e, std::pair<Edge*, bool>* ret0, std::pair<Edge*, bool>* ret1) const
{
	if (m_e01.first == e) {
		*ret0 = m_e12;
		*ret1 = m_e20;
	}
	else if (m_e12.first == e) {
		*ret0 = m_e20;
		*ret1 = m_e01;
	}
	else if (m_e20.first == e) {
		*ret0 = m_e01;
		*ret1 = m_e12;
	}
	else 
		assert(0);
}

//
// Internal
//

bool DelaunayTriangulation::InsertNode(Node* n)
{
	Triangle *curr = m_root;
	while (true)
	{
		if (curr->m_leaf)
		{
			int type = Math2D::CheckPosInTriangle(n->m_pos, curr->m_nodes[0]->m_pos, curr->m_nodes[1]->m_pos, curr->m_nodes[2]->m_pos);
			switch (type)
			{
			case 0:
				curr->m_e01.first->InsertNode(n);
				return true;
			case 1:
				curr->m_e12.first->InsertNode(n);
				return true;
			case 2:
				curr->m_e20.first->InsertNode(n);
				return true;
			case 3:
				return false;
			case 4:
				curr->InsertNode(n);
				return true;
			default:
				assert(0);
			}
		}
		else
		{
			for (size_t i = 0; i < curr->m_children.size(); ++i)
			{
				Triangle* child(curr->m_children[i]);
				int type = Math2D::CheckPosInTriangle(n->m_pos, child->m_nodes[0]->m_pos, child->m_nodes[1]->m_pos, child->m_nodes[2]->m_pos);
				if (type != -1)
				{
					curr = child;
					break;
				}
			}
		}
	}
}

void DelaunayTriangulation::DeleteNode(const Node* n)
{
	std::set<Triangle*> totDel;
	GetAllDelTris(n, totDel);

	std::set<Node*> reinsertNodes;
	GetReinsertNodes(totDel, reinsertNodes);

	UpdateConnection(totDel);

	UpdateEdgeInfo();

	Reinsert(reinsertNodes);

	std::set<Triangle*>::iterator itrTri = totDel.begin();
	for ( ; itrTri != totDel.end(); ++itrTri)
		delete *itrTri;

	//// debug
	//std::ofstream fout("c:/5_1.txt", std::ios_base::app);
	//float N = m_nodes.size(), K = totDel.size(), R = reinsertNodes.size();
	//fout << "N: " << N << '\t' << "K: " << K << '\t'
	//	<< "R: " << R << '\t' << "max(RlogN, K) / NlogN = " 
	//	<< std::max(R * log(N), K) / (N * log(N)) * 100 << "%" << std::endl;
	////
}

//			p0	
//		---------
//		|		|
//		---------
//	p1				p2

void DelaunayTriangulation::InitSurrondTri(const std::vector<sm::vec2>& src)
{
	sm::rect rect;
	Math2D::GetMBR(src, &rect);
	sm::vec2 sz = rect.Size();
	float extend = std::max(sz.x, sz.y) * BOUND_EXTEND_TIMES;
	sm::vec2 center(0.5f * (rect.xmin + rect.xmax), 0.5f * (rect.ymin + rect.ymax));

	sm::vec2 p0(center.x, center.y + extend), 
		p1(center.x - extend, center.y - extend), 
		p2(center.x + extend, center.y - extend);

	m_root = new Triangle;
	// p0 - p1 - p2 is anticlockwise for triangle
	m_root->InitTri(new Node(p0, true), new Node(p1, true), new Node(p2, true));
}

void DelaunayTriangulation::InitSrcNodes(const std::vector<sm::vec2>& src)
{
	for(size_t i = 0; i < src.size(); ++i)
		m_nodes.insert(new Node(src[i]));
}

void DelaunayTriangulation::BuildTriangulationStruct()
{
	std::vector<Node*> nodes;
	nodes.reserve(m_nodes.size());
	copy(m_nodes.begin(), m_nodes.end(), back_inserter(nodes));
	Random::RandomPermutation(nodes);

	for (size_t i = 0; i < nodes.size(); ++i)
		InsertNode(nodes[i]);
}

void DelaunayTriangulation::SetAllDestTris(bool bFixBound /*= true*/)
{
	GetAllLeafTris();
	SetNodeConnection();
	if (bFixBound)
	{
		SetRemovedTris();
		CheckSingleNode();
	}

	std::vector<Triangle*> updateTris;
	updateTris.reserve(m_tris.size());
	for (size_t i = 0; i < m_tris.size(); ++i)
		if (!m_tris[i]->m_removed)
			updateTris.push_back(m_tris[i]);
	m_tris = updateTris;
}

void DelaunayTriangulation::GetAllLeafTris()
{
	m_tris.clear();

	std::set<Triangle*> visited;
	std::queue<Triangle*> buffer;
	visited.insert(m_root);
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Triangle* tri = buffer.front(); buffer.pop();
		if (!tri->m_leaf)
			for (size_t i = 0; i < tri->m_children.size(); ++i)
			{
				Triangle* child(tri->m_children[i]);
				std::pair<std::set<Triangle*>::iterator, bool> state
					= visited.insert(child);
				if (state.second)
					buffer.push(child);
			}
		else
		{
			if (!tri->IsFake())
				m_tris.push_back(tri);
		}
	}
}

void DelaunayTriangulation::SetNodeConnection()
{
	std::set<Node*, NodeCmp>::iterator itrSet = m_nodes.begin();
	for ( ; itrSet != m_nodes.end(); ++itrSet)
		(*itrSet)->m_edges.clear();

	for (size_t i = 0; i < m_tris.size(); ++i)
	{
		Triangle* tri = m_tris[i];

		if (tri->m_nodes[0]->m_edges.empty())
			tri->m_nodes[0]->m_edges.push_back(tri->m_e01.first);
		if (tri->m_nodes[1]->m_edges.empty())
			tri->m_nodes[1]->m_edges.push_back(tri->m_e12.first);
		if (tri->m_nodes[2]->m_edges.empty())
			tri->m_nodes[2]->m_edges.push_back(tri->m_e20.first);
	}

	std::set<Node*, NodeCmp>::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr)
	{
		Node* n(*itr);
		if (n->m_edges.empty())
			continue;
		Edge *start(n->m_edges.front()), *currEdge = start;
		do 
		{
			const Triangle* currTri = (currEdge->m_start == n ? currEdge->LeftTri() : currEdge->RightTri());

			Edge* lastEdge = currEdge;
			bool bSet = false;
			if ((currTri->m_e01.first->m_start == n || currTri->m_e01.first->m_end == n) && currTri->m_e01.first != lastEdge)
			{
				assert(!bSet);
				currEdge = currTri->m_e01.first;
				bSet = true;
			}
			if ((currTri->m_e12.first->m_start == n || currTri->m_e12.first->m_end == n) && currTri->m_e12.first != lastEdge)
			{
				assert(!bSet);
				currEdge = currTri->m_e12.first;
				bSet = true;
			}
			if ((currTri->m_e20.first->m_start == n || currTri->m_e20.first->m_end == n) && currTri->m_e20.first != lastEdge)
			{
				assert(!bSet);
				currEdge = currTri->m_e20.first;
				bSet = true;
			}
			assert(bSet);

			if (currEdge != start)
				n->m_edges.push_back(currEdge);
			else
				break;
		} while (true);
	}
}

void DelaunayTriangulation::SetRemovedTris()
{
	float threshold = GetAvgEdgesLen() * BOUND_FIX_THRESHOLD_TIMES;

	for (size_t i = 0; i < m_tris.size(); ++i)
		m_tris[i]->m_removed = false;

	for (size_t i = 0; i < m_tris.size(); ++i)
	{
		if (m_tris[i]->m_removed)
			continue;
		std::queue<Triangle*> buffer;
		buffer.push(m_tris[i]);
		while (!buffer.empty())
		{
			Triangle* tri = buffer.front(); buffer.pop();

			if (tri->m_removed)
				continue;
			if (tri->IsFake())
			{
				tri->m_removed = true;
				continue;
			}

			if (tri->m_e01.first->IsMargin() && tri->m_e01.first->Length() > threshold)
			{
				tri->m_removed = true;
				buffer.push(tri->m_e12.second ? tri->m_e12.first->RightTri() : tri->m_e12.first->LeftTri());
				buffer.push(tri->m_e20.second ? tri->m_e20.first->RightTri() : tri->m_e20.first->LeftTri());
			}
			if (tri->m_e12.first->IsMargin() && tri->m_e12.first->Length() > threshold)
			{
				tri->m_removed = true;
				buffer.push(tri->m_e01.second ? tri->m_e01.first->RightTri() : tri->m_e01.first->LeftTri());
				buffer.push(tri->m_e20.second ? tri->m_e20.first->RightTri() : tri->m_e20.first->LeftTri());
			}
			if (tri->m_e20.first->IsMargin() && tri->m_e20.first->Length() > threshold)
			{
				tri->m_removed = true;
				buffer.push(tri->m_e01.second ? tri->m_e01.first->RightTri() : tri->m_e01.first->LeftTri());
				buffer.push(tri->m_e12.second ? tri->m_e12.first->RightTri() : tri->m_e12.first->LeftTri());
			}
		}
	}
}

float DelaunayTriangulation::GetAvgEdgesLen() const
{
	float totLen = 0;
	for (size_t i = 0; i < m_tris.size(); ++i)
	{
		Triangle* tri(m_tris[i]);
		totLen += tri->m_e01.first->Length();
		totLen += tri->m_e12.first->Length();
		totLen += tri->m_e20.first->Length();
	}
	return totLen / (m_tris.size() * 3);
}

void DelaunayTriangulation::CheckSingleNode()
{
	std::set<Node*, NodeCmp>::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr)
	{
		bool bSingle = true;
		for (size_t i = 0; i < (*itr)->m_edges.size(); ++i)
			if (!(*itr)->m_edges[i]->IsErase())
				bSingle = false;

		if (bSingle)
		{
			Triangle* minEdgeTri = NULL;
			float minLen = FLT_MAX;
			for (size_t i = 0; i < (*itr)->m_edges.size(); ++i)
			{
				Edge* e((*itr)->m_edges[i]);
				Triangle* tri;

				tri = e->LeftTri();
				if (!tri->IsFake() && tri->m_removed)
				{
					sm::vec2 other = e->LeftNode()->m_pos;
					float maxLenEdge = std::max(
						sm::dis_pos_to_pos(e->m_start->m_pos, other),
						sm::dis_pos_to_pos(e->m_end->m_pos, other)
						);
					if (maxLenEdge < minLen)
					{
						minEdgeTri = tri;
						minLen = maxLenEdge;
					}
				}

				tri = e->RightTri();
				if (!tri->IsFake() && tri->m_removed)
				{
					sm::vec2 other = e->RightNode()->m_pos;
					float maxLenEdge = std::max(
						sm::dis_pos_to_pos(e->m_start->m_pos, other),
						sm::dis_pos_to_pos(e->m_end->m_pos, other)
						);
					if (maxLenEdge < minLen)
					{
						minEdgeTri = tri;
						minLen = maxLenEdge;
					}
				}
			}

			assert(minEdgeTri);
			minEdgeTri->m_removed = false;
		}
	}
}

void DelaunayTriangulation::GetAllDelTris(const Node* n, std::set<Triangle*>& totDel) const
{
	Triangle *curr = m_root, *split = NULL;
	while (true)
	{
		if (!curr->m_leaf)
		{
			Triangle* child(curr->m_children.front());
			if (child->m_nodes[0] == n || child->m_nodes[1] == n || child->m_nodes[2] == n)
			{
				split = curr;
				break;
			}

			for (size_t i = 0; i < curr->m_children.size(); ++i)
			{
				Triangle* child(curr->m_children[i]);
				int type = Math2D::CheckPosInTriangle(n->m_pos, child->m_nodes[0]->m_pos, child->m_nodes[1]->m_pos, child->m_nodes[2]->m_pos);
				if (type != -1)
				{
					curr = child;
					break;
				}
			}
		}
		else
		{
			assert(curr->m_nodes[0] != n && curr->m_nodes[1] != n && curr->m_nodes[2] != n);
			break;
		}
	}

	assert(split);
	std::queue<Triangle*> buffer;
	for (size_t i = 0; i < split->m_children.size(); ++i)
		buffer.push(split->m_children[i]);

	while (!buffer.empty())
	{
		Triangle* del = buffer.front(); buffer.pop();
		std::pair<std::set<Triangle*>::iterator, bool> state = totDel.insert(del);
		if (!state.second)
			continue;

		for (size_t i = 0; i < del->m_children.size(); ++i)
		{
			Triangle* child(del->m_children[i]);
			if (totDel.find(child) == totDel.end())
				buffer.push(del->m_children[i]);
		}
	}

}

void DelaunayTriangulation::GetReinsertNodes(const std::set<Triangle*>& totDel, std::set<Node*>& reinsertNodes) const
{
	std::set<Triangle*>::const_iterator itr = totDel.begin();
	for ( ; itr != totDel.end(); ++itr)
	{
		Triangle* curr(*itr);
		if (curr->m_leaf)
			continue;

		bool bSplit = true;
		for (size_t i = 0; i < curr->m_children.size(); ++i)
			if (curr->m_children[i]->m_parent.size() > 1) {
				bSplit = false;
				break;
			}

			if (bSplit)
			{
				for (size_t i = 0; i < 3; ++i)
				{
					Node* n = curr->m_children.front()->m_nodes[i];
					if (n != curr->m_nodes[0] && n != curr->m_nodes[1] && n != curr->m_nodes[2])
					{
						reinsertNodes.insert(n);
						break;
					}
				}
			}
	}
}

void DelaunayTriangulation::UpdateConnection(const std::set<Triangle*>& totDel)
{
	std::set<Triangle*>::const_iterator itr = totDel.begin();
	for ( ; itr != totDel.end(); ++itr)
	{
		Triangle *curr(*itr);

		std::vector<Triangle*>::iterator itrParent = curr->m_parent.begin();
		for ( ; itrParent != curr->m_parent.end(); ++itrParent)
		{
			std::vector<Triangle*>::iterator itrChild = (*itrParent)->m_children.begin();
			for ( ; itrChild != (*itrParent)->m_children.end(); ++itrChild)
				if (*itrChild == curr) {
					(*itrParent)->m_children.erase(itrChild);
					break;
				}
				if ((*itrParent)->m_children.empty())
					(*itrParent)->m_leaf = true;
		}
	}
}

void DelaunayTriangulation::UpdateEdgeInfo()
{
	std::set<Triangle*> tris;
	std::queue<Triangle*> buffer;

	tris.insert(m_root);
	buffer.push(m_root);
	while (!buffer.empty())
	{
		Triangle* curr = buffer.front(); buffer.pop();
		if (curr->m_leaf)
		{
			UpdateEdgeInfo(curr, curr->m_e01);
			UpdateEdgeInfo(curr, curr->m_e12);
			UpdateEdgeInfo(curr, curr->m_e20);
		}
		else
			for (size_t i = 0; i < curr->m_children.size(); ++i)
			{
				Triangle* child(curr->m_children[i]);
				std::pair<std::set<Triangle*>::iterator, bool> state
					= tris.insert(child);
				if (state.second)
					buffer.push(child);
			}
	}
}

void DelaunayTriangulation::UpdateEdgeInfo(Triangle* tri, const std::pair<Edge*, bool>& edge)
{
	std::pair<Triangle*, Node*>& dest(edge.second ? edge.first->m_left : edge.first->m_right);
	dest.first = tri;

	size_t i = 0;
	for ( ; i < 3; ++i)
		if (tri->m_nodes[i] != edge.first->m_start && tri->m_nodes[i] != edge.first->m_end)
		{
			dest.second = tri->m_nodes[i];
			break;
		}
		assert(i != 3);
}

void DelaunayTriangulation::Reinsert(const std::set<Node*>& reinsertNodes)
{
	if (!reinsertNodes.empty())
	{
		std::vector<Node*> nodes;
		nodes.reserve(reinsertNodes.size());
		copy(reinsertNodes.begin(), reinsertNodes.end(), back_inserter(nodes));
		Random::RandomPermutation(nodes);
		for (size_t i = 0; i < nodes.size(); ++i)
			InsertNode(nodes[i]);
	}
}

}