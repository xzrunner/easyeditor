#ifndef _SPRITE2_SCALE9_SPRITE_H_
#define _SPRITE2_SCALE9_SPRITE_H_

#include "S2_Sprite.h"
#include "Scale9.h"

namespace s2
{

class RenderParams;

class Scale9Sprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Scale9Sprite();
	Scale9Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Scale9Sprite* Clone() const;

	const Scale9& GetScale9() const { return m_s9; }

	VI_DUMMY_FUNC

protected:
	Scale9 m_s9;

}; // Scale9Sprite

}

#endif // _SPRITE2_SCALE9_SPRITE_H_