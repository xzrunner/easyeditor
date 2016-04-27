#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include "Symbol.h"
#include "MeshTrans.h"

#include <ee/Sprite.h>
#include <ee/Vector.h>

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
	virtual bool Update(int version) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	const ee::Vector& GetSpeed() const { return m_speed; }
	void SetSpeed(const ee::Vector& spd) { m_speed = spd; }

	void SetTween(Sprite* begin, Sprite* end, float process);

	const MeshTrans& GetMeshTrans() const { return m_trans; }
	MeshTrans& GetMeshTrans() { return m_trans; }

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	ee::Vector m_speed;

	mutable MeshTrans m_trans;

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_