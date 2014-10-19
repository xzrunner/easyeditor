#ifndef _LIBSKETCH_SPRITE_H_
#define _LIBSKETCH_SPRITE_H_

#include "Symbol.h"
#include "Quaternion.h"

namespace libsketch
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(Symbol* symbol);
	Sprite(const Sprite& sprite);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* clone() const;

	//
	// d2d::ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);
	virtual void loadBodyFromFile();

	const vec3& GetPos3() const { return m_pos3; }

	const Quaternion& GetOri3() const { return m_ori3; }
	void SetOri3(const Quaternion& ori) { m_ori3 = ori; }

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	vec3 m_pos3;
	Quaternion m_ori3;

}; // Sprite

}

#endif // _LIBSKETCH_SPRITE_H_