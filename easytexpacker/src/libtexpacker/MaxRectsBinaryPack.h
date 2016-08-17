#ifndef _EASYTEXPACKER_MAX_RECTS_BINARY_PACK_H_
#define _EASYTEXPACKER_MAX_RECTS_BINARY_PACK_H_

#include "Rect.h"
#include "Sprite.h"

#include <vector>

namespace etexpacker
{

class MaxRectsBinaryPack
{
public:
	MaxRectsBinaryPack();
	~MaxRectsBinaryPack();

	RectSize GetSize() const;

	void Pack(const std::vector<RectSize>& rects, std::vector<Rect>& output);

private:
	void ResetRoot(int width, int height);

private:
	class Node
	{
	public:
		Node();
		Node(int width, int height);
		~Node();

		Node* insert(Sprite* spr, int flag);

		int Width() const { return m_rc.width; }
		int Height() const { return m_rc.height; }

	private:
		Node* m_child[2];

		Sprite* m_spr;

		Rect m_rc;

	}; // Node

private:
	Node* m_root;

}; // MaxRectsBinaryPack

}

#endif // _EASYTEXPACKER_MAX_RECTS_BINARY_PACK_H_