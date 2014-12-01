#ifndef _LIBPACKER_MAX_RECTS_BINARY_PACK_H_
#define _LIBPACKER_MAX_RECTS_BINARY_PACK_H_

#include "Rect.h"

namespace libpacker
{

class MaxRectsBinaryPack
{
public:
	MaxRectsBinaryPack();
	~MaxRectsBinaryPack();

	void Pack(const std::vector<RectSize>& rects, std::vector<Rect>& output);

private:
	struct Node
	{
		Node* child[2];
		RectSize size;

		Node();
		~Node();

		Node* insert(Rect* result, int flag);
	};

private:
	Node* m_root;

}; // MaxRectsBinaryPack

}

#endif // _LIBPACKER_MAX_RECTS_BINARY_PACK_H_