#ifndef _LR_QUADTREE_H_
#define _LR_QUADTREE_H_

#include <SM_Rect.h>

#include <vector>

namespace ee { class Rect; class Sprite; }

namespace lr
{

class Quadtree
{
public:
	Quadtree(const sm::rect& rect);
	~Quadtree();

	void Insert(const ee::SprConstPtr& spr);
	
	std::vector<const ee::SprPtr> Query(const sm::rect& rect) const;

	void DebugDraw() const;

	void SelectNode(const sm::vec2& pos);

private:
	class Node
	{
	public:
		Node(const sm::rect& rect);
		~Node();

		void Insert(const ee::SprConstPtr& spr);

		bool IsLeaf() const;

	private:
		bool IsIntersect(const sm::rect& rect) const;
		bool IsContain(const sm::vec2& pos) const;
		bool IsContain(const ee::SprConstPtr& spr) const;

		bool NeedSplit() const;
		void Split();

		float GetContainArea(const ee::SprConstPtr& spr) const;

	private:
		static const int MAX_COUNT;
		static const float MAX_AREA;
		static const int MIN_GRID;

	private:
		sm::rect m_rect;

		// 0 1
		// 2 3
		Node* m_children[4];

		std::vector<const ee::SprPtr> m_sprs;

		friend class Quadtree;

	}; // Node

private:
	Node* m_root;

	Node* m_selected;

}; // Quadtree

}

#endif // _LR_QUADTREE_H_