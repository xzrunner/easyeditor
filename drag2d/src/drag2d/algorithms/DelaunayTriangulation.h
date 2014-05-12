#pragma once

#include <vector>
#include <set>

#include "common/Vector.h"

namespace d2d
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

		const std::vector<Triangle*>& getAllTris() const { return m_tris; }
		const std::set<Node*, NodeCmp>& getAllNodes() const { return m_nodes; }
		void getAllTrisInRegion(std::vector<std::vector<Vector> >& triBounds, const std::vector<Vector>& region) const;
		void getAllTrisInRegion(std::vector<Vector>& triBounds, const std::vector<Vector>& region) const;

		bool insertNode(const Vector& p);
		void deleteNodes(const std::vector<Vector>& pos);

		bool isBoundNode(const Vector& n) const;
		void getBoundLinePos(std::vector<std::vector<Vector> >& bounds) const;

		void getSurroundPos(const Vector& pos, std::vector<Vector>& surround) const;

	public:
		class Edge;
		class Node
		{
		public:
			Node(const Vector& pos, bool bFake = false) 
				: m_pos(pos), m_bFake(bFake) {}

			void getSurroundTris(std::vector<const Triangle*>& tris) const;

			bool isMargin() const;

		public:
			const Vector m_pos;
			const bool m_bFake;

		private:
			std::vector<Edge*> m_edges;

			friend class DelaunayTriangulation;
		}; // Node

		class Edge
		{
		public:
			// only has left tri, this is the edge of bound tri
			Edge(Node* in0, Node* in1, const std::pair<Triangle*, Node*>& t)
				: m_nStart(in0), m_nEnd(in1), m_tLeft(t) {}
			// from node0 to node1, tri0 is left, tri1 is right
			Edge(Node* node0, Node* node1, const std::pair<Triangle*, Node*>& tri0, const std::pair<Triangle*, Node*>& tri1)
				: m_nStart(node0), m_nEnd(node1), m_tLeft(tri0), m_tRight(tri1) {}

			void insertNode(Node* n);

			// left triangle is new
			void legalizeEdge(bool checkRightTri);

			void resetTri(const std::pair<Triangle*, Node*>& tri, bool bLeft) {
				bLeft ? m_tLeft = tri : m_tRight = tri;
			}

			float length() const;

			bool isMargin() const { 
				size_t c = 0;
				if (m_tLeft.first->isFake() || m_tLeft.first->m_bRemoved) ++c;
				if (m_tRight.first->isFake() || m_tRight.first->m_bRemoved) ++c;
				return c == 1;
			}

			bool isErase() const {
				size_t c = 0;
				if (m_tLeft.first->isFake() || m_tLeft.first->m_bRemoved) ++c;
				if (m_tRight.first->isFake() || m_tRight.first->m_bRemoved) ++c;
				return c == 2;
			}

			bool isLeafEdge() const { return m_tLeft.first->m_bLeaf && m_tRight.first->m_bLeaf; }

			Triangle* leftTri() const { return m_tLeft.first; }
			Triangle* rightTri() const { return m_tRight.first; }
			Node* leftNode() const { return m_tLeft.second; }
			Node* rightNode() const { return m_tRight.second; }

		public:
			Node *const m_nStart, *const m_nEnd;

		private:
			std::pair<Triangle*, Node*> m_tLeft, m_tRight;

			friend class DelaunayTriangulation;
		}; // Edge

		class Triangle
		{
		public:
			Triangle();
			// create edge info, only used for init boundary triangle
			void initTri(Node* in0, Node* in1, Node* in2);

			void insertNode(Node* n);

			void getNodesPos(std::vector<Vector>& pos) const {
				for (size_t i = 0; i < 3; ++i)
					pos.push_back(m_nodes[i]->m_pos);
			}

			bool isFake() const { return m_nodes[0]->m_bFake 
				|| m_nodes[1]->m_bFake || m_nodes[2]->m_bFake; }

		private:
			void initTri(Node* in0, Node* in1, Node* in2, const std::pair<Edge*, bool>& e01, 
				const std::pair<Edge*, bool>& e12, const std::pair<Edge*, bool>& e20);
			void addChildren(Triangle* t0, Triangle* t1);
			void addChildren(Triangle* t0, Triangle* t1, Triangle* t2);
			void getNextTwoEdge(const Edge* e, std::pair<Edge*, bool>* ret0, std::pair<Edge*, bool>* ret1) const;

		private:
			bool m_bLeaf, m_bRemoved;

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
		bool insertNode(Node* n);
		void deleteNode(const Node* n);

		void initSurrondTri(const std::vector<Vector>& src);
		void initSrcNodes(const std::vector<Vector>& src);
		void buildTriangulationStruct();
		void setAllDestTris(bool bFixBound = true);

		void getAllLeafTris();
		void setNodeConnection();
		void setRemovedTris();
		float getAvgEdgesLen() const;
		void checkSingleNode();

		void getAllDelTris(const Node* n, std::set<Triangle*>& totDel) const;
		void getReinsertNodes(const std::set<Triangle*>& totDel, std::set<Node*>& reinsertNodes) const;
		void updateConnection(const std::set<Triangle*>& totDel);
		void updateEdgeInfo();
		void updateEdgeInfo(Triangle* tri, const std::pair<Edge*, bool>& edge);
		void reinsert(const std::set<Node*>& reinsertNodes);

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
		Triangle* m_rootTri;

		std::set<Node*, NodeCmp> m_nodes;

		std::vector<Triangle*> m_tris;

	}; // DelaunayTriangulation
}

