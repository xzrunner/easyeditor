#ifndef _SPRITE2_MASK_SPRITE_H_
#define _SPRITE2_MASK_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class MaskSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	MaskSprite();
	MaskSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual MaskSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const RenderParams& params);
	virtual Sprite* FetchChild(const std::string& name) const;

	VI_DUMMY_FUNC

}; // MaskSprite

}

#endif // _SPRITE2_MASK_SPRITE_H_