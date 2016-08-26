#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "S2_Sprite.h"
#include "Particle3d.h"

struct p3d_sprite;

namespace s2
{

class Particle3dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle3dSprite();
	Particle3dSprite(const Particle3dSprite& spr);
	Particle3dSprite& operator = (const Particle3dSprite& spr);
	Particle3dSprite(Symbol* sym);
	virtual ~Particle3dSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Particle3dSprite* Clone() const;
	virtual bool Update(const RenderParams& params, float dt);

	void Draw(const s2::RenderParams& params) const;

	void SetOuterMatrix(const sm::mat4& mat) const;

	bool IsAlone() const { return m_alone; }
	void SetAlone(bool alone);

	bool IsLocalModeDraw() const;
	void SetLocalModeDraw(bool local);

	p3d_sprite* GetP3dSpr() const { return m_spr; }

protected:
	void CreateSpr();

protected:
	p3d_sprite* m_spr;

	sm::Quaternion m_dir;

	bool m_alone;
	bool m_reuse;

	mutable s2::P3dRenderParams m_rp;

	bool m_spr_ref;

	VI_DUMMY_FUNC

}; // Particle3dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_