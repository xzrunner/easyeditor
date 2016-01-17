#ifndef _GLUE_SPRITE_H_
#define _GLUE_SPRITE_H_

#include "math/Matrix.h"

namespace glue
{

class Symbol;

class Sprite
{
public:
	Sprite(Symbol* sym);
	~Sprite();

	virtual void Update();

	const Symbol& GetSymbol() const { return *m_sym; }

	mat4 GetTransMatrix() const;

private:
	vec2 m_pos;

	Symbol* m_sym;

}; // Sprite

}

#endif // _GLUE_SPRITE_H_