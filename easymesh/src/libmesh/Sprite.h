#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include <drag2d.h>

#include "Symbol.h"

namespace emesh
{

class Sprite : public d2d::Sprite
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
	virtual void SetSymbol(d2d::Symbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual void BuildBounding();

	Shape* getShape() {
		return m_symbol ? m_symbol->getShape() : NULL; 
	}
	const d2d::Image* getImage() const {
		return m_symbol ? m_symbol->getImage() : NULL;
	}
	d2d::Image* getImage() {
		return m_symbol ? m_symbol->getImage() : NULL;
	}

	const d2d::Vector& GetSpeed() const { return m_speed; }
	void SetSpeed(const d2d::Vector& spd) { m_speed = spd; }

	void SetTween(Sprite* begin, Sprite* end, float process);

	static d2d::Sprite* Create(d2d::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	d2d::Vector m_speed;

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_