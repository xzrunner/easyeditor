#include "DelaunayTriangulation.h"

#include <queue>
#include <assert.h>

#include "common/Random.h"
#include "common/Math.h"
#include "common/tools.h"

namespace d2d
{

DelaunayTriangulation::DelaunayTriangulation(const std::vector<Vector>& src, bool bFixBound /*= true*/)
{
	if (src.size() >= 3)
	{
		initSurrondTri(src);
		initSrcNodes(src);
		buildTriangulationStruct();
		setAllDestTris(bFixBound);
	}
}

DelaunayTriangulation::~DelaunayTriangulation()
{
	for_each(m_nodes.begin(), m_nodes.end(), DeletePointerFunctor<Node>());

	std::set<Triangle*> tris;
	std::set<Edge*> edges;
	std::queue<Triangle*> buffer;

	tris.insert(m_rootTri);
	buffer.push(m_rootTri);
	while (!buffer.empty())
	{
		Triangle* tri = buffer.front(); buffer.pop();
		if (!tri->m_bLeaf)
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

void DelaunayTriangulation::getAllTrisInRegion(std::vector<std::vector<Vector> >& triBounds, 
											   const std::vector<Vector>& region) const
{
	std::vector<Triangle*> tris = getAllTris();
	for (size_t i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* src = tris[i];
		std::vector<Vector> bound;
		src->getNodesPos(bound);

		Vector p = Math::getTriGravityCenter(bound[0], bound[1], bound[2]);
		if (Math::isPointInArea(p, region))
			triBounds.push_back(bound);
	}
}

void DelaunayTriangulation::getAllTrisInRegion(std::vector<Vector>& triBounds, const std::vector<Vector>& region) const
{
	std::vector<Triangle*> tris = getAllTris();
	for (size_t i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* src = tris[i];
		std::vector<Vector> bound;
		src->getNodesPos(bound);

		Vector p = Math::getTriGravityCenter(bound[0], bound[1], bound[2]);
		if (Math::isPointInArea(p, region))
			copy(bound.begin(), bound.end(), back_inserter(triBounds));
	}
}

bool DelaunayTriangulation::insertNode(const Vector& p)
{
	int type = Math::checkPosInTriangle(p, m_rootTri->m_nodes[0]->m_pos, m_rootTri->m_nodes[1]->m_pos, m_rootTri->m_nodes[2]->m_pos);
	if (type == -1 || type == 3)
		return false;

	Node* n = new Node(p);
	std::pair<std::set<Node*, NodeCmp>::iterator, bool> state = m_nodes.insert(n);
	if (state.second)
	{
		if (insertNode(n))
		{
			// todo: can update partially
			setAllDestTris();

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

void DelaunayTriangulation::deleteNodes(const std::vector<Vector>& pos)
{
	for (size_t i = 0; i < pos.size(); ++i)
	{
		Node n(pos[i]);
		std::set<Node*, NodeCmp>::iterator itr = m_nodes.find(&n);
		if (itr == m_nodes.end())
			continue;

		deleteNode(*itr);
		delete *itr;
		m_nodes.erase(itr);
	}
	// todo: can update partially
	setAllDestTris();
}

bool DelaunayTriangulation::isBoundNode(const Vector& n) const
{
	Node tmp(n);
	std::set<Node*, NodeCmp>::const_iterator itr = m_nodes.find(&tmp);
	if (itr == m_nodes.end())
		return false;
	else
		return (*itr)->isMargin();
}

void DelaunayTriangulation::getBoundLinePos(std::vector<std::vector<Vector> >& bounds) const
{
	std::set<Vector, PosDownCmp> leftPos;
	std::set<Node*, NodeCmp>::const_iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr)
		if (isBoundNode((*itr)->m_pos))
			leftPos.insert((*itr)->m_pos);

	while (!leftPos.empty())
	{
		std::vector<Vector> bound;
		bound.push_back(*leftPos.begin());
		leftPos.erase(leftPos.begin());
		while (true)
		{
			Node nCurr(bound.back());
			std::set<Node*, NodeCmp>::const_iterator ptrCurrNode = m_nodes.find(&nCurr);
			assert(ptrCurrNode != m_nodes.end());

			float minAngle = FLT_MAX;
			Vector nextPos;
			for (size_t i = 0; i < (*ptrCurrNode)->m_edges.size(); ++i)
			{
				Edge* e((*ptrCurrNode)->m_edges[i]);
				if (e->isMargin())
				{
					Vector otherPos(e->m_nStart == *ptrCurrNode ? e->m_nEnd->m_pos : e->m_nStart->m_pos);

					if (bound.size() >= 2 && otherPos == bound[bound.size() - 2])
						continue;

					float angle;
					if (bound.size() < 2)
						angle = Math::getLineAngle(bound[bound.size() - 1], otherPos);
					else
						angle = Math::getAngle(bound[bound.size() - 1], bound[bound.size() - 2], otherPos);
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

void DelaunayTriangulation::getSurroundPos(const Vector& pos, std::vector<Vector>& surround) const
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
			surround.push_back(e->m_nStart == n ? e->m_nEnd->m_pos : e->m_nStart->m_pos);
		}
	}
}

//
// class DelaunayTriangulation::Node
//

void DelaunayTriangulation::Node::getSurroundTris(std::vector<const Triangle*>& tris) const
{
	for (size_t i = 0; i < m_edges.size(); ++i)
	{
		const Edge* e(m_edges[i]);
		tris.push_back(e->m_nStart == this ? e->leftTri() : e->rightTri());
	}
}

bool DelaunayTriangulation::Node::isMargin() const
{
	for (size_t i = 0; i < m_edges.size(); ++i)
		if (m_edges[i]->isMargin())
			return true;
	return false;
}

//
// class DelaunayTriangulation::Edge
//

void DelaunayTriangulation::Edge::
	insertNode(Node* n)
{
	Triangle *tLeftStart  = new Triangle, *tLeftEnd  = new Triangle,
		*tRightStart = new Triangle, *tRightEnd = new Triangle;
	Edge *eLeft  = new Edge(n, m_tLeft.second, std::make_pair(tLeftStart, m_nStart), std::make_pair(tLeftEnd, m_nEnd)),
		*eRight = new Edge(n, m_tRight.second, std::make_pair(tRightEnd, m_nEnd), std::make_pair(tRightStart, m_nStart)),
		*eStart = new Edge(m_nStart, n, std::make_pair(tLeftStart, m_tLeft.second), std::make_pair(tRightStart, m_tRight.second)),
		*eEnd   = new Edge(n, m_nEnd, std::make_pair(tLeftEnd, m_tLeft.second), std::make_pair(tRightEnd, m_tRight.second));

	std::pair<Edge*, bool> eLeftStart, eLeftEnd, eRightStart, eRightEnd;
	leftTri()->getNextTwoEdge(this, &eLeftEnd, &eLeftStart);
	rightTri()->getNextTwoEdge(this, &eRightStart, &eRightEnd);

	tLeftStart->initTri(n, m_tLeft.second, m_nStart, std::make_pair(eLeft, true), eLeftStart, std::make_pair(eStart, true));
	tLeftEnd->initTri(n, m_nEnd, m_tLeft.second, std::make_pair(eEnd, true), eLeftEnd, std::make_pair(eLeft, false));
	tRightStart->initTri(n, m_nStart, m_tRight.second, std::make_pair(eStart, false), eRightStart, std::make_pair(eRight, false));
	tRightEnd->initTri(n, m_tRight.second, m_nEnd, std::make_pair(eRight, true), eRightEnd, std::make_pair(eEnd, false));

	if (eLeftStart.second)
		eLeftStart.first->resetTri(std::make_pair(tLeftStart, n), true);
	else
		eLeftStart.first->resetTri(std::make_pair(tLeftStart, n), false);
	if (eLeftEnd.second)
		eLeftEnd.first->resetTri(std::make_pair(tLeftEnd, n), true);
	else
		eLeftEnd.first->resetTri(std::make_pair(tLeftEnd, n), false);
	if (eRightStart.second)
		eRightStart.first->resetTri(std::make_pair(tRightStart, n), true);
	else
		eRightStart.first->resetTri(std::make_pair(tRightStart, n), false);
	if (eRightEnd.second)
		eRightEnd.first->resetTri(std::make_pair(tRightEnd, n), true);
	else
		eRightEnd.first->resetTri(std::make_pair(tRightEnd, n), false);

	leftTri()->addChildren(tLeftStart, tLeftEnd);
	rightTri()->addChildren(tRightStart, tRightEnd);

	eLeftStart.first->legalizeEdge(eLeftStart.second);
	eLeftEnd.first->legalizeEdge(eLeftEnd.second);
	eRightStart.first->legalizeEdge(eRightStart.second);
	eRightEnd.first->legalizeEdge(eRightEnd.second);
}

void DelaunayTriangulation::Edge::
	legalizeEdge(bool checkRightTri)
{
	if (m_nStart->m_bFake && m_nEnd->m_bFake)
		return;

	Vector checkPos(m_tLeft.second->m_pos), fixPos(m_tRight.second->m_pos);
	if (checkRightTri)
		std::swap(checkPos, fixPos);

	Vector center;
	Math::getCircumcenter(m_nStart->m_pos, m_nEnd->m_pos, fixPos, &center);
	if (Math::getDistance(checkPos, center) < Math::getDistance(fixPos, center))
	{
		assert(leftTri()->m_bLeaf && rightTri()->m_bLeaf);

		if (checkRightTri) {
			Triangle *newTriLeft = new Triangle, *newTriRight = new Triangle;
			Edge* newEdge = new Edge(m_tLeft.second, m_tRight.second, std::make_pair(newTriLeft, m_nEnd), std::make_pair(newTriRight, m_nStart));

			std::pair<Edge*, bool> triLeftFix, triRightFix, triLeftCheck, triRightCheck;
			leftTri()->getNextTwoEdge(this, &triLeftFix, &triRightFix);
			rightTri()->getNextTwoEdge(this, &triRightCheck, &triLeftCheck);

			if (triLeftFix.second)
				triLeftFix.first->resetTri(std::make_pair(newTriLeft, m_tRight.second), true);
			else
				triLeftFix.first->resetTri(std::make_pair(newTriLeft, m_tRight.second), false);
			if (triRightFix.second)
				triRightFix.first->resetTri(std::make_pair(newTriRight, m_tRight.second), true);
			else
				triRightFix.first->resetTri(std::make_pair(newTriRight, m_tRight.second), false);
			if (triLeftCheck.second)
				triLeftCheck.first->resetTri(std::make_pair(newTriLeft, m_tLeft.second), true);
			else
				triLeftCheck.first->resetTri(std::make_pair(newTriLeft, m_tLeft.second), false);
			if (triRightCheck.second)
				triRightCheck.first->resetTri(std::make_pair(newTriRight, m_tLeft.second), true);
			else
				triRightCheck.first->resetTri(std::make_pair(newTriRight, m_tLeft.second), false);

			newTriLeft->initTri(m_tLeft.second, m_tRight.second, m_nEnd, std::make_pair(newEdge, true), triLeftCheck, triLeftFix);
			newTriRight->initTri(m_tRight.second, m_tLeft.second, m_nStart, std::make_pair(newEdge, false), triRightFix, triRightCheck);

			leftTri()->addChildren(newTriLeft, newTriRight);
			rightTri()->addChildren(newTriLeft, newTriRight);

			triLeftCheck.first->legalizeEdge(triLeftCheck.second);
			triRightCheck.first->legalizeEdge(triRightCheck.second);
		}
		else {
			Triangle *newTriLeft = new Triangle, *newTriRight = new Triangle;
			Edge* newEdge = new Edge(m_tRight.second, m_tLeft.second, std::make_pair(newTriLeft, m_nStart), std::make_pair(newTriRight, m_nEnd));

			std::pair<Edge*, bool> triLeftFix, triRightFix, triLeftCheck, triRightCheck;
			rightTri()->getNextTwoEdge(this, &triLeftFix, &triRightFix);
			leftTri()->getNextTwoEdge(this, &triRightCheck, &triLeftCheck);

			if (triLeftFix.second)
				triLeftFix.first->resetTri(std::make_pair(newTriLeft, m_tLeft.second), true);
			else
				triLeftFix.first->resetTri(std::make_pair(newTriLeft, m_tLeft.second), false);
			if (triRightFix.second)
				triRightFix.first->resetTri(std::make_pair(newTriRight, m_tLeft.second), true);
			else
				triRightFix.first->resetTri(std::make_pair(newTriRight, m_tLeft.second), false);
			if (triLeftCheck.second)
				triLeftCheck.first->resetTri(std::make_pair(newTriLeft, m_tRight.second), true);
			else
				triLeftCheck.first->resetTri(std::make_pair(newTriLeft, m_tRight.second), false);
			if (triRightCheck.second)
				triRightCheck.first->resetTri(std::make_pair(newTriRight, m_tRight.second), true);
			else
				triRightCheck.first->resetTri(std::make_pair(newTriRight, m_tRight.second), false);

			newTriLeft->initTri(m_tRight.second, m_tLeft.second, m_nStart, std::make_pair(newEdge, true), triLeftCheck, triLeftFix);
			newTriRight->initTri(m_tLeft.second, m_tRight.second, m_nEnd, std::make_pair(newEdge, false), triRightFix, triRightCheck);

			leftTri()->addChildren(newTriLeft, newTriRight);
			rightTri()->addChildren(newTriLeft, newTriRight);

			triLeftCheck.first->legalizeEdge(triLeftCheck.second);
			triRightCheck.first->legalizeEdge(triRightCheck.second);
		}
	}
}

float DelaunayTriangulation::Edge::length() const
{
	return Math::getDistance(m_nStart->m_pos, m_nEnd->m_pos);
}

//
// class DelaunayTriangulation::Triangle
//

DelaunayTriangulation::Triangle::Triangle()
	: m_bLeaf(true), m_bRemoved(false)
{ 
	m_nodes[0] = m_nodes[1] = m_nodes[2] = NULL;
	m_e01.first = m_e12.first = m_e20.first = NULL;
}

void DelaunayTriangulation::Triangle::
	initTri(Node* in0, Node* in1, Node* in2)
{
	m_nodes[0] = in0;
	m_nodes[1] = in1;
	m_nodes[2] = in2;
	m_e01 = std::make_pair(new Edge(m_nodes[0], m_nodes[1], std::make_pair(this, m_nodes[2])), true);
	m_e12 = std::make_pair(new Edge(m_nodes[1], m_nodes[2], std::make_pair(this, m_nodes[0])), true); 
	m_e20 = std::make_pair(new Edge(m_nodes[2], m_nodes[0], std::make_pair(this, m_nodes[1])), true);
	m_bLeaf = true;
}

void DelaunayTriangulation::Triangle::
	insertNode(Node* n)
{
	Triangle *t01 = new Triangle, *t12 = new Triangle, *t20 = new Triangle;
	Edge *e0n = new Edge(m_nodes[0], n, std::make_pair(t20, m_nodes[2]), std::make_pair(t01, m_nodes[1])),
		*e1n = new Edge(m_nodes[1], n, std::make_pair(t01, m_nodes[0]), std::make_pair(t12, m_nodes[2])), 
		*e2n = new Edge(m_nodes[2], n, std::make_pair(t12, m_nodes[1]), std::make_pair(t20, m_nodes[0]));
	t01->initTri(m_nodes[0], m_nodes[1], n, m_e01, std::make_pair(e1n, true), std::make_pair(e0n, false));
	t12->initTri(m_nodes[1], m_nodes[2], n, m_e12, std::make_pair(e2n, true), std::make_pair(e1n, false));
	t20->initTri(m_nodes[2], m_nodes[0], n, m_e20, std::make_pair(e0n, true), std::make_pair(e2n, false));

	if (m_e01.second)
		m_e01.first->resetTri(std::make_pair(t01, n), true);
	else
		m_e01.first->resetTri(std::make_pair(t01, n), false);
	if (m_e12.second)
		m_e12.first->resetTri(std::make_pair(t12, n), true);
	else
		m_e12.first->resetTri(std::make_pair(t12, n), false);
	if (m_e20.second)
		m_e20.first->resetTri(std::make_pair(t20, n), true);
	else
		m_e20.first->resetTri(std::make_pair(t20, n), false);

	addChildren(t01, t12, t20);

	m_e01.first->legalizeEdge(m_e01.second);
	m_e12.first->legalizeEdge(m_e12.second);
	m_e20.first->legalizeEdge(m_e20.second);
}

void DelaunayTriangulation::Triangle::
	initTri(Node* pN0, Node* pN1, Node* pN2, const std::pair<Edge*, bool>& pE01, 
		const std::pair<Edge*, bool>& pE12, const std::pair<Edge*, bool>& pE20)
{
	m_nodes[0]  = pN0;
	m_nodes[1]  = pN1;
	m_nodes[2]  = pN2;
	m_e01 = pE01;
	m_e12 = pE12; 
	m_e20 = pE20;
	m_bLeaf = true;
};

void DelaunayTriangulation::Triangle::
	addChildren(Triangle* t0, Triangle* t1)
{
	assert(m_bLeaf);
	m_bLeaf = false;
	t0->m_parent.push_back(this);
	t1->m_parent.push_back(this);
	m_children.push_back(t0);
	m_children.push_back(t1);
}

void DelaunayTriangulation::Triangle::
	addChildren(Triangle* t0, Triangle* t1, Triangle* t2)
{
	assert(m_bLeaf);
	m_bLeaf = false;
	t0->m_parent.push_back(this);
	t1->m_parent.push_back(this);
	t2->m_parent.push_back(this);
	m_children.push_back(t0);
	m_children.push_back(t1);
	m_children.push_back(t2);
}

void DelaunayTriangulation::Triangle::
	getNextTwoEdge(const Edge* e, std::pair<Edge*, bool>* ret0, std::pair<Edge*, bool>* ret1) const
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

bool DelaunayTriangulation::insertNode(Node* n)
{
	Triangle *curr = m_rootTri;
	while (true)
	{
		if (curr->m_bLeaf)
		{
			int type = Math::checkPosInTriangle(n->m_pos, curr->m_nodes[0]->m_pos, curr->m_nodes[1]->m_pos, curr->m_nodes[2]->m_pos);
			switch (type)
			{
			case 0:
				curr->m_e01.first->insertNode(n);
				return true;
			case 1:
				curr->m_e12.first->insertNode(n);
				return true;
			case 2:
				curr->m_e20.first->insertNode(n);
				return true;
			case 3:
				return false;
			case 4:
				curr->insertNode(n);
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
				int type = Math::checkPosInTriangle(n->m_pos, child->m_nodes[0]->m_pos, child->m_nodes[1]->m_pos, child->m_nodes[2]->m_pos);
				if (type != -1)
				{
					curr = child;
					break;
				}
			}
		}
	}
}

void DelaunayTriangulation::deleteNode(const Node* n)
{
	std::set<Triangle*> totDel;
	getAllDelTris(n, totDel);

	std::set<Node*> reinsertNodes;
	getReinsertNodes(totDel, reinsertNodes);

	updateConnection(totDel);

	updateEdgeInfo();

	reinsert(reinsertNodes);

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

void DelaunayTriangulation::initSurrondTri(const std::vector<Vector>& src)
{
	Rect rect;
	Math::getMBR(src, &rect);
	float extend = std::max(rect.xLength(), rect.yLength()) * BOUND_EXTEND_TIMES;
	Vector center(0.5f * (rect.xMin + rect.xMax), 0.5f * (rect.yMin + rect.yMax));

	Vector p0(center.x, center.y + extend), 
		p1(center.x - extend, center.y - extend), 
		p2(center.x + extend, center.y - extend);

	m_rootTri = new Triangle;
	// p0 - p1 - p2 is anticlockwise for triangle
	m_rootTri->initTri(new Node(p0, true), new Node(p1, true), new Node(p2, true));
}

void DelaunayTriangulation::initSrcNodes(const std::vector<Vector>& src)
{
	for(size_t i = 0; i < src.size(); ++i)
		m_nodes.insert(new Node(src[i]));
}

void DelaunayTriangulation::buildTriangulationStruct()
{
	std::vector<Node*> nodes;
	nodes.reserve(m_nodes.size());
	copy(m_nodes.begin(), m_nodes.end(), back_inserter(nodes));
	Random::RandomPermutation(nodes);

	for (size_t i = 0; i < nodes.size(); ++i)
		insertNode(nodes[i]);
}

void DelaunayTriangulation::setAllDestTris(bool bFixBound /*= true*/)
{
	getAllLeafTris();
	setNodeConnection();
	if (bFixBound)
	{
		setRemovedTris();
		checkSingleNode();
	}

	std::vector<Triangle*> updateTris;
	updateTris.reserve(m_tris.size());
	for (size_t i = 0; i < m_tris.size(); ++i)
		if (!m_tris[i]->m_bRemoved)
			updateTris.push_back(m_tris[i]);
	m_tris = updateTris;
}

void DelaunayTriangulation::getAllLeafTris()
{
	m_tris.clear();

	std::set<Triangle*> visited;
	std::queue<Triangle*> buffer;
	visited.insert(m_rootTri);
	buffer.push(m_rootTri);
	while (!buffer.empty())
	{
		Triangle* tri = buffer.front(); buffer.pop();
		if (!tri->m_bLeaf)
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
			if (!tri->isFake())
				m_tris.push_back(tri);
		}
	}
}

void DelaunayTriangulation::setNodeConnection()
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
			const Triangle* currTri = (currEdge->m_nStart == n ? currEdge->leftTri() : currEdge->rightTri());

			Edge* lastEdge = currEdge;
			bool bSet = false;
			if ((currTri->m_e01.first->m_nStart == n || currTri->m_e01.first->m_nEnd == n) && currTri->m_e01.first != lastEdge)
			{
				assert(!bSet);
				currEdge = currTri->m_e01.first;
				bSet = true;
			}
			if ((currTri->m_e12.first->m_nStart == n || currTri->m_e12.first->m_nEnd == n) && currTri->m_e12.first != lastEdge)
			{
				assert(!bSet);
				currEdge = currTri->m_e12.first;
				bSet = true;
			}
			if ((currTri->m_e20.first->m_nStart == n || currTri->m_e20.first->m_nEnd == n) && currTri->m_e20.first != lastEdge)
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

void DelaunayTriangulation::setRemovedTris()
{
	float threshold = getAvgEdgesLen() * BOUND_FIX_THRESHOLD_TIMES;

	for (size_t i = 0; i < m_tris.size(); ++i)
		m_tris[i]->m_bRemoved = false;

	for (size_t i = 0; i < m_tris.size(); ++i)
	{
		if (m_tris[i]->m_bRemoved)
			continue;
		std::queue<Triangle*> buffer;
		buffer.push(m_tris[i]);
		while (!buffer.empty())
		{
			Triangle* tri = buffer.front(); buffer.pop();

			if (tri->m_bRemoved)
				continue;
			if (tri->isFake())
			{
				tri->m_bRemoved = true;
				continue;
			}

			if (tri->m_e01.first->isMargin() && tri->m_e01.first->length() > threshold)
			{
				tri->m_bRemoved = true;
				buffer.push(tri->m_e12.second ? tri->m_e12.first->rightTri() : tri->m_e12.first->leftTri());
				buffer.push(tri->m_e20.second ? tri->m_e20.first->rightTri() : tri->m_e20.first->leftTri());
			}
			if (tri->m_e12.first->isMargin() && tri->m_e12.first->length() > threshold)
			{
				tri->m_bRemoved = true;
				buffer.push(tri->m_e01.second ? tri->m_e01.first->rightTri() : tri->m_e01.first->leftTri());
				buffer.push(tri->m_e20.second ? tri->m_e20.first->rightTri() : tri->m_e20.first->leftTri());
			}
			if (tri->m_e20.first->isMargin() && tri->m_e20.first->length() > threshold)
			{
				tri->m_bRemoved = true;
				buffer.push(tri->m_e01.second ? tri->m_e01.first->rightTri() : tri->m_e01.first->leftTri());
				buffer.push(tri->m_e12.second ? tri->m_e12.first->rightTri() : tri->m_e12.first->leftTri());
			}
		}
	}
}

float DelaunayTriangulation::getAvgEdgesLen() const
{
	float totLen = 0;
	for (size_t i = 0; i < m_tris.size(); ++i)
	{
		Triangle* tri(m_tris[i]);
		totLen += tri->m_e01.first->length();
		totLen += tri->m_e12.first->length();
		totLen += tri->m_e20.first->length();
	}
	return totLen / (m_tris.size() * 3);
}

void DelaunayTriangulation::checkSingleNode()
{
	std::set<Node*, NodeCmp>::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr)
	{
		bool bSingle = true;
		for (size_t i = 0; i < (*itr)->m_edges.size(); ++i)
			if (!(*itr)->m_edges[i]->isErase())
				bSingle = false;

		if (bSingle)
		{
			Triangle* minEdgeTri = NULL;
			float minLen = FLT_MAX;
			for (size_t i = 0; i < (*itr)->m_edges.size(); ++i)
			{
				Edge* e((*itr)->m_edges[i]);
				Triangle* tri;

				tri = e->leftTri();
				if (!tri->isFake() && tri->m_bRemoved)
				{
					Vector other = e->leftNode()->m_pos;
					float maxLenEdge = std::max(
						Math::getDistance(e->m_nStart->m_pos, other),
						Math::getDistance(e->m_nEnd->m_pos, other)
						);
					if (maxLenEdge < minLen)
					{
						minEdgeTri = tri;
						minLen = maxLenEdge;
					}
				}

				tri = e->rightTri();
				if (!tri->isFake() && tri->m_bRemoved)
				{
					Vector other = e->rightNode()->m_pos;
					float maxLenEdge = std::max(
						Math::getDistance(e->m_nStart->m_pos, other),
						Math::getDistance(e->m_nEnd->m_pos, other)
						);
					if (maxLenEdge < minLen)
					{
						minEdgeTri = tri;
						minLen = maxLenEdge;
					}
				}
			}

			assert(minEdgeTri);
			minEdgeTri->m_bRemoved = false;
		}
	}
}

void DelaunayTriangulation::getAllDelTris(const Node* n, std::set<Triangle*>& totDel) const
{
	Triangle *curr = m_rootTri, *split = NULL;
	while (true)
	{
		if (!curr->m_bLeaf)
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
				int type = Math::checkPosInTriangle(n->m_pos, child->m_nodes[0]->m_pos, child->m_nodes[1]->m_pos, child->m_nodes[2]->m_pos);
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

void DelaunayTriangulation::getReinsertNodes(const std::set<Triangle*>& totDel, std::set<Node*>& reinsertNodes) const
{
	std::set<Triangle*>::const_iterator itr = totDel.begin();
	for ( ; itr != totDel.end(); ++itr)
	{
		Triangle* curr(*itr);
		if (curr->m_bLeaf)
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

void DelaunayTriangulation::updateConnection(const std::set<Triangle*>& totDel)
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
					(*itrParent)->m_bLeaf = true;
		}
	}
}

void DelaunayTriangulation::updateEdgeInfo()
{
	std::set<Triangle*> tris;
	std::queue<Triangle*> buffer;

	tris.insert(m_rootTri);
	buffer.push(m_rootTri);
	while (!buffer.empty())
	{
		Triangle* curr = buffer.front(); buffer.pop();
		if (curr->m_bLeaf)
		{
			updateEdgeInfo(curr, curr->m_e01);
			updateEdgeInfo(curr, curr->m_e12);
			updateEdgeInfo(curr, curr->m_e20);
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

void DelaunayTriangulation::updateEdgeInfo(Triangle* tri, const std::pair<Edge*, bool>& edge)
{
	std::pair<Triangle*, Node*>& dest(edge.second ? edge.first->m_tLeft : edge.first->m_tRight);
	dest.first = tri;

	size_t i = 0;
	for ( ; i < 3; ++i)
		if (tri->m_nodes[i] != edge.first->m_nStart && tri->m_nodes[i] != edge.first->m_nEnd)
		{
			dest.second = tri->m_nodes[i];
			break;
		}
		assert(i != 3);
}

void DelaunayTriangulation::reinsert(const std::set<Node*>& reinsertNodes)
{
	if (!reinsertNodes.empty())
	{
		std::vector<Node*> nodes;
		nodes.reserve(reinsertNodes.size());
		copy(reinsertNodes.begin(), reinsertNodes.end(), back_inserter(nodes));
		Random::RandomPermutation(nodes);
		for (size_t i = 0; i < nodes.size(); ++i)
			insertNode(nodes[i]);
	}
}

} // d2d