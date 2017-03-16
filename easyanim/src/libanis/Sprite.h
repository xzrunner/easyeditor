#ifndef _LIBANIS_SPRITE_H_
#define _LIBANIS_SPRITE_H_

#include <ee/Sprite.h>

namespace libanis
{

class Symbol;

class Sprite : public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	int m_idx;

}; // Sprite

}

#endif // _LIBANIS_SPRITE_H_