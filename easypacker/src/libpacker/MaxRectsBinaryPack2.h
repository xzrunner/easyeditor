#ifndef _LIBPACKER_MAX_RECTS_BINARY_PACK2_H_
#define _LIBPACKER_MAX_RECTS_BINARY_PACK2_H_

#include "Rect.h"
#include "Sprite.h"

#include <drag2d.h>

namespace libpacker
{

class MaxRectsBinaryPack2
{
public:
	MaxRectsBinaryPack2();
	~MaxRectsBinaryPack2();

	void Pack(const std::vector<RectSize>& rects, std::vector<Rect>& output);

private:
	void InitRoot(int w, int h);

	bool Insert(const libpacker::Sprite* sprite) const;

private:
	d2d::TPNode* m_root;

}; // MaxRectsBinaryPack2

}

#endif // _LIBPACKER_MAX_RECTS_BINARY_PACK2_H_