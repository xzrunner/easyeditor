#ifndef _SPRITE2_PARTICLE2D_SPRITE_H_
#define _SPRITE2_PARTICLE2D_SPRITE_H_

#include "S2_Sprite.h"
#include "Particle2d.h"

struct p2d_emitter;

namespace s2
{

class Particle2dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle2dSprite();
	Particle2dSprite(const Particle2dSprite& spr);
	Particle2dSprite& operator = (const Particle2dSprite& spr);
	Particle2dSprite(Symbol* sym);
	virtual ~Particle2dSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Particle2dSprite* Clone() const;
	virtual bool Update(const RenderParams& params);

	void Draw(const RenderParams& params) const;

	void SetMatrix(const sm::mat4& mat) const { m_mat = mat; }

	bool GetLoop() const;
	void SetLoop(bool loop);

	bool GetLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

protected:
	p2d_emitter* m_et;

	mutable sm::mat4 m_mat;

	VI_DUMMY_FUNC

}; // Particle2dSprite

}

#endif // _SPRITE2_PARTICLE2D_SPRITE_H_