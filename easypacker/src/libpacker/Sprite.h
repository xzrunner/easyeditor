#ifndef _LIBPACKER_SPRITE_H_
#define _LIBPACKER_SPRITE_H_

namespace libpacker
{

struct Sprite
{
	Sprite(const RectSize* size, Rect* pos) 
		: size(size), pos(pos) {}

	const RectSize* size;
	Rect* pos;
}; // Sprite

struct SpriteCmp
{
	bool operator() (const Sprite& s0, const Sprite& s1) const {
		return s0.size->width * s0.size->height > s1.size->width * s1.size->height;
	}
}; // SpriteCmp

}

#endif // _LIBPACKER_SPRITE_H_