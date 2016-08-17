#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include "MeshTrans.h"

#include <ee/Sprite.h>

#include <sprite2/MeshSprite.h>

namespace emesh
{

class Symbol;

class Sprite : public s2::MeshSprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite(Symbol* sym);
	virtual ~Sprite();

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual bool Update(const s2::RenderParams& params, float dt) { return true; }

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual Sprite* EEClone() const { return new Sprite(*this); }

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	const sm::vec2& GetSpeed() const { return m_speed; }
	void SetSpeed(const sm::vec2& spd) { m_speed = spd; }

	void SetTween(Sprite* begin, Sprite* end, float process);

	const MeshTrans& GetMeshTrans() const { return m_trans; }
	MeshTrans& GetMeshTrans() { return m_trans; }

	const ee::Symbol* GetBaseSym() const { return m_base; }
	void SetBaseSym(const ee::Symbol* sym);

	bool OnlyDrawBound() const { return m_only_draw_bound; }
	void SetOnlyDrawBound(bool only_draw_bound) { m_only_draw_bound = only_draw_bound; }

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	sm::vec2 m_speed;

	mutable MeshTrans m_trans;

	const ee::Symbol* m_base;

	bool m_only_draw_bound;

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_