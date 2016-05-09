#ifndef _EASYEDITOR_TRAPEZOIDAL_DECOMPOSITION_H_
#define _EASYEDITOR_TRAPEZOIDAL_DECOMPOSITION_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class TrapezoidalDec
{
public:
	TrapezoidalDec();
	~TrapezoidalDec();

	void Insert(const sm::vec2& p0, const sm::vec2& p1);

private:
	void InitRoot(const sm::vec2& lpos, const sm::vec2& rpos);
	void InsertSeg(const sm::vec2& lpos, const sm::vec2& rpos);

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

		virtual INode* Query(const sm::vec2& pos) = 0;

		void SetChild0(INode* c) { m_child0 = c; c->m_parent = this; }
		void SetChild1(INode* c) { m_child1 = c; c->m_parent = this; }

	public:
		INode* m_parent;
		INode *m_child0, *m_child1;

	}; // INode

	class Node : public INode
	{
	public:
		Node(const sm::vec2& pos) : m_pos(pos) {}

		virtual Type GetType() const { return e_node; }

		virtual INode* Query(const sm::vec2& pos);

	public:
		sm::vec2 m_pos;
	}; // Node

	class Seg : public INode
	{
	public:
		Seg(Node* left, Node* right) 
			: m_left(left), m_right(right) {}

		virtual Type GetType() const { return e_seg; }

		virtual INode* Query(const sm::vec2& pos);

	public:
		Node *m_left, *m_right;

	}; // Seg

	class Area : public INode
	{
	public:
		Area(Seg* top, Seg* bottom, Node* left, Node* right)
			: m_top(top), m_bottom(bottom), m_left(left), m_right(right) {}

		virtual Type GetType() const { return e_area; }

		virtual INode* Query(const sm::vec2& pos);

	public:
		Seg *m_top, *m_bottom;
		Node *m_left, *m_right;

	}; // Area

private:
	INode* m_root;

}; // TrapezoidalDec

}

#endif // _EASYEDITOR_TRAPEZOIDAL_DECOMPOSITION_H_