#ifndef _SPRITE2_ICON_SPRITE_H_
#define _SPRITE2_ICON_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class IconSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	IconSprite();
	IconSprite(const IconSprite& spr);
	IconSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual IconSprite* Clone() const;

	void SetProcess(float process) { m_process = process; }
	float GetProcess() const { return m_process; }

	VI_DUMMY_FUNC

protected:
	float m_process;

}; // IconSprite

}

#endif // _SPRITE2_ICON_SPRITE_H_