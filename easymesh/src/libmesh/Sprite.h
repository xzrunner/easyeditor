#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include <drag2d.h>

#include "Symbol.h"

namespace emesh
{

class Sprite : public d2d::ISprite
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
	// ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);

	virtual void load(const Json::Value& val);
	virtual void store(Json::Value& val) const;

	virtual void buildBounding();

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

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	d2d::Vector m_speed;

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_