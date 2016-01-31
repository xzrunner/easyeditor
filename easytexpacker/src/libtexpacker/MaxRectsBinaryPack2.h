#ifndef _LIBTEXPACKER_MAX_RECTS_BINARY_PACK2_H_
#define _LIBTEXPACKER_MAX_RECTS_BINARY_PACK2_H_

#include "Rect.h"
#include "Sprite.h"
#include "typedef.h"



namespace libtexpacker
{

class MaxRectsBinaryPack2
{
public:
	MaxRectsBinaryPack2();
	~MaxRectsBinaryPack2();

	void GetSize(std::vector<RectSize>& sizes) const;

	void Pack(PACK_STRATEGY strategy, int static_size, int max_size, int min_size, 
		const std::vector<RectSize>& rects, std::vector<Rect>& output);

private:
	int CalTotArea(const std::vector<Sprite>& list) const;

	ee::TPNode* NewRoot(int w, int h);

	bool Insert(ee::TPNode* root, const libtexpacker::Sprite* sprite, int tex_id = 0) const;

	void PackAuto(const std::vector<Sprite>& sprites, int area);
	void PackSquare(const std::vector<Sprite>& sprites, int area);

	void PackSquareMulti(std::vector<Sprite>& sprites, int static_size);

	void PackSquareMultiAuto(std::vector<Sprite>& sprites, int area, int max_size, int min_size);
	bool MergeSquareMultiAuto(std::vector<Sprite>& sprites, int max_size);
	void SortRoots(const std::vector<Sprite>& sprites);

private:
	class NodeCmp
	{
	public:
		bool operator() (const ee::TPNode* n0, ee::TPNode* n1) const;
	}; // NodeCmp

private:
	std::vector<ee::TPNode*> m_roots;

}; // MaxRectsBinaryPack2

}

#endif // _LIBTEXPACKER_MAX_RECTS_BINARY_PACK2_H_