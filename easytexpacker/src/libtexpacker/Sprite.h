#ifndef _EASYTEXPACKER_SPRITE_H_
#define _EASYTEXPACKER_SPRITE_H_

#include <algorithm>

namespace etexpacker
{

struct Sprite
{
	Sprite(const RectSize* size, Rect* pos) 
		: size(size), pos(pos) {}

	const RectSize* size;
	Rect* pos;
}; // Sprite

enum SortStrategy
{
	e_area,
	e_max_edge,
	e_tot_edges
};

struct SpriteCmp
{
	SpriteCmp(SortStrategy strategy) : m_strategy(strategy) {}

	bool operator() (const Sprite& s0, const Sprite& s1) const {
		int w0 = s0.size->width, h0 = s0.size->height;
		int w1 = s1.size->width, h1 = s1.size->height;
		
		if (m_strategy == e_area) {
			return w0 * h0 > w1 * h1;
		} else if (m_strategy == e_max_edge) {
			return std::max(w0, h0) > std::max(w1, h1);
		} else {
			return w0 + h0 > w1 + h1;
		}
	}

	SortStrategy m_strategy;

}; // SpriteCmp

}

#endif // _EASYTEXPACKER_SPRITE_H_