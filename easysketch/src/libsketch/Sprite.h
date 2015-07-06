#ifndef _LIBSKETCH_SPRITE_H_
#define _LIBSKETCH_SPRITE_H_

#include "Symbol.h"
#include "Quaternion.h"
#include "Vector.h"

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
	virtual Sprite* Clone() const;

	//
	// d2d::ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);
	virtual void loadBodyFromFile();

	const vec3& GetPos3() const { return m_pos3; }
	void SetPos3(const vec3& pos) { m_pos3 = pos; }
	void Translate3(const vec3& offset) { m_pos3 += offset; }

	const Quaternion& GetOri3() const { return m_ori3; }
	void SetOri3(const Quaternion& ori) { m_ori3 = ori; }
	void Rotate3(const Quaternion& delta) {
		m_ori3.Rotate(delta);
		m_ori3 = delta.Rotated(m_ori3);
	}

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