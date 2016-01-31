#ifndef _LR_QUADTREE_H_
#define _LR_QUADTREE_H_



namespace lr
{

class Quadtree
{
public:
	Quadtree(const ee::Rect& rect);
	~Quadtree();

	void Insert(const ee::Sprite* spr);
	
	std::vector<const ee::Sprite*> Query(const ee::Rect& rect) const;

	void DebugDraw() const;

	void SelectNode(const ee::Vector& pos);

private:
	class Node
	{
	public:
		Node(const ee::Rect& rect);
		~Node();

		void Insert(const ee::Sprite* spr);

		bool IsLeaf() const;

	private:
		bool IsIntersect(const ee::Rect& rect) const;
		bool IsContain(const ee::Vector& pos) const;
		bool IsContain(const ee::Sprite* spr) const;

		bool NeedSplit() const;
		void Split();

		float GetContainArea(const ee::Sprite* spr) const;

	private:
		static const int MAX_COUNT;
		static const float MAX_AREA;
		static const int MIN_GRID;

	private:
		ee::Rect m_rect;

		// 0 1
		// 2 3
		Node* m_children[4];

		std::vector<const ee::Sprite*> m_sprites;

		friend class Quadtree;

	}; // Node

private:
	Node* m_root;

	Node* m_selected;

}; // Quadtree

}

#endif // _LR_QUADTREE_H_