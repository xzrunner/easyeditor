#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include "Symbol.h"
#include "MeshTrans.h"

#include <ee/Sprite.h>

namespace emesh
{

class Sprite : public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& s);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(float dt, const sm::mat4& mat) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	const sm::vec2& GetSpeed() const { return m_speed; }
	void SetSpeed(const sm::vec2& spd) { m_speed = spd; }

	void SetTween(Sprite* begin, Sprite* end, float process);

	const MeshTrans& GetMeshTrans() const { return m_trans; }
	MeshTrans& GetMeshTrans() { return m_trans; }

	const ee::Symbol* GetBaseSym() const { return m_base; }
	void SetBaseSym(const ee::Symbol* sym) { ee::obj_assign(m_base, sym); }

	bool OnlyDrawBound() const { return m_only_draw_bound; }
	void SetOnlyDrawBound(bool only_draw_bound) { m_only_draw_bound = only_draw_bound; }

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	sm::vec2 m_speed;

	mutable MeshTrans m_trans;

	const ee::Symbol* m_base;

	bool m_only_draw_bound;

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_