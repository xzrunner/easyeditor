#ifndef _LIBPACKER_MAX_RECTS_BINARY_PACK2_H_
#define _LIBPACKER_MAX_RECTS_BINARY_PACK2_H_

#include "Rect.h"
#include "Sprite.h"
#include "typedef.h"

#include <drag2d.h>

namespace libpacker
{

class MaxRectsBinaryPack2
{
public:
	MaxRectsBinaryPack2();
	~MaxRectsBinaryPack2();

	void GetSize(std::vector<RectSize>& sizes) const;

	void Pack(PACK_STRATEGY strategy, const std::vector<RectSize>& rects, 
		std::vector<Rect>& output);

private:
	int CalTotArea(const std::vector<Sprite>& list) const;

	d2d::TPNode* NewRoot(int w, int h);

	bool Insert(d2d::TPNode* root, const libpacker::Sprite* sprite, int tex_id = 0) const;

	void PackAuto(const std::vector<Sprite>& sprites, int area,
		std::vector<Rect>& output);
	void PackSquare(const std::vector<Sprite>& sprites, int area,
		std::vector<Rect>& output);
	void PackSquareMulti(const std::vector<Sprite>& sprites, int area,
		std::vector<Rect>& output);

private:
	std::vector<d2d::TPNode*> m_roots;

}; // MaxRectsBinaryPack2

}

#endif // _LIBPACKER_MAX_RECTS_BINARY_PACK2_H_