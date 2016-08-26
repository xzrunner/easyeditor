#ifndef _SPRITE2_TRAIL_SPRITE_H_
#define _SPRITE2_TRAIL_SPRITE_H_

#include "S2_Sprite.h"
#include "Trail.h"

struct t2d_emitter;

namespace s2
{

class TrailSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TrailSprite();
	TrailSprite(Symbol* sym);
	virtual ~TrailSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual TrailSprite* Clone() const;
	virtual bool Update(const RenderParams& params, float dt);

	void Draw(const RenderParams& params) const;

protected:
	t2d_emitter* m_et;

	mutable TrailRenderParams m_rp;

	VI_DUMMY_FUNC

}; // TrailSprite

}

#endif // _SPRITE2_TRAIL_SPRITE_H_