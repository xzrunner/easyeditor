#ifndef _LIBPACKER_MAX_RECTS_BINARY_PACK_H_
#define _LIBPACKER_MAX_RECTS_BINARY_PACK_H_

#include "Rect.h"
#include "Sprite.h"

#include <vector>

namespace libpacker
{

class MaxRectsBinaryPack
{
public:
	MaxRectsBinaryPack();
	~MaxRectsBinaryPack();

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

		Node* insert(Sprite* sprite, int flag);

	private:
		Node* m_child[2];

		Sprite* m_sprite;

		Rect m_rc;

	}; // Node

private:
	Node* m_root;

}; // MaxRectsBinaryPack

}

#endif // _LIBPACKER_MAX_RECTS_BINARY_PACK_H_