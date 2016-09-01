#ifndef _SPRITE2_MESH_SPRITE_H_
#define _SPRITE2_MESH_SPRITE_H_

#include "S2_Sprite.h"
#include "MeshTransform.h"

namespace s2
{

class MeshSymbol;

class MeshSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	MeshSprite();
	MeshSprite(const MeshSprite& mesh);
	MeshSprite& operator = (const MeshSprite& mesh);
	MeshSprite(MeshSymbol* sym);
	virtual ~MeshSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual MeshSprite* Clone() const;
	virtual bool Update(const RenderParams& params) { return true; }

	void SetTween(MeshSprite* begin, MeshSprite* end, float process);

	const sm::vec2& GetSpeed() const { return m_speed; }
	void SetSpeed(const sm::vec2& spd) { m_speed = spd; }

	const MeshTransform& GetMeshTrans() const { return m_trans; }
	MeshTransform& GetMeshTrans() { return m_trans; }

	const Symbol* GetBaseSym() const { return m_base; }
	void SetBaseSym(const Symbol* sym);

	bool OnlyDrawBound() const { return m_only_draw_bound; }
	void SetOnlyDrawBound(bool only_draw_bound) { m_only_draw_bound = only_draw_bound; }

protected:
	sm::vec2 m_speed;

	MeshTransform m_trans;

	const Symbol* m_base;

	bool m_only_draw_bound;

	VI_DUMMY_FUNC

}; // MeshSprite

}

#endif // _SPRITE2_MESH_SPRITE_H_