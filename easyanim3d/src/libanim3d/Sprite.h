#ifndef _EASYANIM3D_SPRITE_H_
#define _EASYANIM3D_SPRITE_H_

#include "Symbol.h"

namespace eanim3d
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// ICloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);

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

#endif // _EASYANIM3D_SPRITE_H_