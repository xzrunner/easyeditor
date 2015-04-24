#ifndef _D2D_TRAPEZOIDAL_DECOMPOSITION_H_
#define _D2D_TRAPEZOIDAL_DECOMPOSITION_H_

#include "common/Vector.h"

#include <vector>

namespace d2d
{

class TrapezoidalDec
{
public:
	TrapezoidalDec();
	~TrapezoidalDec();

	void Insert(const d2d::Vector& p0, const d2d::Vector& p1);

private:
	void InitRoot(const d2d::Vector& lpos, const d2d::Vector& rpos);
	void InsertSeg(const d2d::Vector& lpos, const d2d::Vector& rpos);

private:
	class INode
	{
	public:
		enum Type
		{
			e_node = 0,
			e_seg,
			e_area,
		};

	public:
		INode() : m_parent(NULL), m_child0(NULL), m_child1(NULL) {}

		virtual Type GetType() const = 0;

		virtual INode* Query(const d2d::Vector& pos) = 0;

		void SetChild0(INode* c) { m_child0 = c; c->m_parent = this; }
		void SetChild1(INode* c) { m_child1 = c; c->m_parent = this; }

	public:
		INode* m_parent;
		INode *m_child0, *m_child1;

	}; // INode

	class Node : public INode
	{
	public:
		Node(const d2d::Vector& pos) : m_pos(pos) {}

		virtual Type GetType() const { return e_node; }

		virtual INode* Query(const d2d::Vector& pos);

	public:
		Vector m_pos;
	}; // Node

	class Seg : public INode
	{
	public:
		Seg(Node* left, Node* right) 
			: m_left(left), m_right(right) {}

		virtual Type GetType() const { return e_seg; }

		virtual INode* Query(const d2d::Vector& pos);

	public:
		Node *m_left, *m_right;

	}; // Seg

	class Area : public INode
	{
	public:
		Area(Seg* top, Seg* bottom, Node* left, Node* right)
			: m_top(top), m_bottom(bottom), m_left(left), m_right(right) {}

		virtual Type GetType() const { return e_area; }

		virtual INode* Query(const d2d::Vector& pos);

	public:
		Seg *m_top, *m_bottom;
		Node *m_left, *m_right;

	}; // Area

private:
	INode* m_root;

}; // TrapezoidalDec

}

#endif // _D2D_TRAPEZOIDAL_DECOMPOSITION_H_