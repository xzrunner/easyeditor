#ifndef _EASYEDITOR_DELAUNAY_TRIANGULATION_H_
#define _EASYEDITOR_DELAUNAY_TRIANGULATION_H_

#include "Vector.h"

#include <vector>
#include <set>

namespace ee
{

class DelaunayTriangulation
{
public:
	class Node;
	class Triangle;
	class NodeCmp;

public:
	DelaunayTriangulation(const std::vector<Vector>& src, bool bFixBound = true);
	~DelaunayTriangulation();

	const std::vector<Triangle*>& GetAllTris() const { return m_tris; }
	const std::set<Node*, NodeCmp>& GetAllNodes() const { return m_nodes; }
	void GetAllTrisInRegion(std::vector<std::vector<Vector> >& triBounds, const std::vector<Vector>& region) const;
	void GetAllTrisInRegion(std::vector<Vector>& triBounds, const std::vector<Vector>& region) const;

	bool InsertNode(const Vector& p);
	void DeleteNodes(const std::vector<Vector>& pos);

	bool IsBoundNode(const Vector& n) const;
	void GetBoundLinePos(std::vector<std::vector<Vector> >& bounds) const;

	void GetSurroundPos(const Vector& pos, std::vector<Vector>& surround) const;

public:
	class Edge;
	class Node
	{
	public:
		Node(const Vector& pos, bool bFake = false) 
			: m_pos(pos), m_fake(bFake) {}

		void GetSurroundTris(std::vector<const Triangle*>& tris) const;

		bool IsMargin() const;

	public:
		const Vector m_pos;
		const bool m_fake;

	private:
		std::vector<Edge*> m_edges;

		friend class DelaunayTriangulation;
	}; // Node

	class Edge
	{
	public:
		// only has left tri, this is the edge of bound tri
		Edge(Node* in0, Node* in1, const std::pair<Triangle*, Node*>& t)
			: m_start(in0), m_end(in1), m_left(t) {}
		// from node0 to node1, tri0 is left, tri1 is right
		Edge(Node* node0, Node* node1, const std::pair<Triangle*, Node*>& tri0, const std::pair<Triangle*, Node*>& tri1)
			: m_start(node0), m_end(node1), m_left(tri0), m_right(tri1) {}

		void InsertNode(Node* n);

		// left triangle is new
		void LegalizeEdge(bool checkRightTri);

		void ResetTri(const std::pair<Triangle*, Node*>& tri, bool bLeft) {
			bLeft ? m_left = tri : m_right = tri;
		}

		float Length() const;

		bool IsMargin() const { 
			size_t c = 0;
			if (m_left.first->IsFake() || m_left.first->m_removed) ++c;
			if (m_right.first->IsFake() || m_right.first->m_removed) ++c;
			return c == 1;
		}

		bool IsErase() const {
			size_t c = 0;
			if (m_left.first->IsFake() || m_left.first->m_removed) ++c;
			if (m_right.first->IsFake() || m_right.first->m_removed) ++c;
			return c == 2;
		}

		bool IsLeafEdge() const { return m_left.first->m_leaf && m_right.first->m_leaf; }

		Triangle* LeftTri() const { return m_left.first; }
		Triangle* RightTri() const { return m_right.first; }
		Node* LeftNode() const { return m_left.second; }
		Node* RightNode() const { return m_right.second; }

	public:
		Node *const m_start, *const m_end;

	private:
		std::pair<Triangle*, Node*> m_left, m_right;

		friend class DelaunayTriangulation;
	}; // Edge

	class Triangle
	{
	public:
		Triangle();
		// create edge info, only used for init boundary triangle
		void InitTri(Node* in0, Node* in1, Node* in2);

		void InsertNode(Node* n);

		void GetNodesPos(std::vector<Vector>& pos) const {
			for (size_t i = 0; i < 3; ++i)
				pos.push_back(m_nodes[i]->m_pos);
		}

		bool IsFake() const { return m_nodes[0]->m_fake 
			|| m_nodes[1]->m_fake || m_nodes[2]->m_fake; }

	private:
		void InitTri(Node* in0, Node* in1, Node* in2, const std::pair<Edge*, bool>& e01, 
			const std::pair<Edge*, bool>& e12, const std::pair<Edge*, bool>& e20);
		void AddChildren(Triangle* t0, Triangle* t1);
		void AddChildren(Triangle* t0, Triangle* t1, Triangle* t2);
		void GetNextTwoEdge(const Edge* e, std::pair<Edge*, bool>* ret0, std::pair<Edge*, bool>* ret1) const;

	private:
		bool m_leaf, m_removed;

		// array nodes anticlockwise of triangle
		// n[0] - n[1] - n[2] is anticlockwise for triangle
		Node* m_nodes[3];

		// m_e01 is connect n0 with n1 ... 
		// bool: sign triangle's edge if same direction with edge itself
		std::pair<Edge*, bool> m_e01, m_e12, m_e20;

		// not have particular order
		std::vector<Triangle*> m_children, m_parent;

		friend class DelaunayTriangulation;
	}; // Triangle

private:
	bool InsertNode(Node* n);
	void DeleteNode(const Node* n);

	void InitSurrondTri(const std::vector<Vector>& src);
	void InitSrcNodes(const std::vector<Vector>& src);
	void BuildTriangulationStruct();
	void SetAllDestTris(bool bFixBound = true);

	void GetAllLeafTris();
	void SetNodeConnection();
	void SetRemovedTris();
	float GetAvgEdgesLen() const;
	void CheckSingleNode();

	void GetAllDelTris(const Node* n, std::set<Triangle*>& totDel) const;
	void GetReinsertNodes(const std::set<Triangle*>& totDel, std::set<Node*>& reinsertNodes) const;
	void UpdateConnection(const std::set<Triangle*>& totDel);
	void UpdateEdgeInfo();
	void UpdateEdgeInfo(Triangle* tri, const std::pair<Edge*, bool>& edge);
	void Reinsert(const std::set<Node*>& reinsertNodes);

private:
	static const size_t BOUND_EXTEND_TIMES			= 1000;
	static const size_t BOUND_FIX_THRESHOLD_TIMES	= 2;

public:
	class NodeCmp
	{
	public:
		bool operator () (const Node* lhs, const Node* rhs) const {
			return lhs->m_pos.x < rhs->m_pos.x 
				|| lhs->m_pos.x == rhs->m_pos.x && lhs->m_pos.y < rhs->m_pos.y;
		}
	}; // NodeCmp

	class PosDownCmp
	{
	public:
		bool operator () (const Vector& lhs, const Vector&  rhs) const {
			return lhs.y > rhs.y
				|| lhs.y == rhs.y && lhs.y < rhs.y;
		}
	}; // PosDownCmp

private:
	Triangle* m_root;

	std::set<Node*, NodeCmp> m_nodes;

	std::vector<Triangle*> m_tris;

}; // DelaunayTriangulation

}

#endif // _EASYEDITOR_DELAUNAY_TRIANGULATION_H_